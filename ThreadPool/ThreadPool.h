//
// Created by 28153 on 2023/8/26.
//

#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

class ThreadPool {
public:
    // 构造函数，接受线程数
    explicit ThreadPool(size_t);

    // 模板函数，用于将函数及其参数加入任务队列，并返回 std::future 对象以获取结果
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type>;

    // 析构函数，优雅地关闭线程池
    ~ThreadPool();

private:
    // 需要跟踪线程以便于 join
    std::vector<std::thread> workers;

    // 任务队列
    std::queue<std::function<void()>> tasks;

    // 同步
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

// 构造函数，启动线程池中的工作线程
inline ThreadPool::ThreadPool(size_t threads)
        : stop(false)
{
    for(size_t i = 0; i < threads; ++i)
        workers.emplace_back(
                [this]
                {
                    for(;;)
                    {
                        std::function<void()> task;

                        {
                            std::unique_lock<std::mutex> lock(this->queue_mutex);

                            // 等待条件满足（有任务或线程池停止）
                            this->condition.wait(lock,
                                                 [this]{ return this->stop || !this->tasks.empty(); });

                            // 如果线程池停止并且任务队列为空，工作线程退出
                            if(this->stop && this->tasks.empty())
                                return;

                            // 取出任务
                            task = std::move(this->tasks.front());
                            this->tasks.pop();
                        }

                        // 执行任务
                        task();
                    }
                }
        );
}

// 将新的工作项加入线程池
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args)
-> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    // 使用 std::packaged_task 包装任务，以便获取结果
    auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    std::future<return_type> res = task->get_future();

    {
        std::unique_lock<std::mutex> lock(queue_mutex);

        // 不允许在线程池停止后继续加入任务
        if(stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        // 将任务加入队列
        tasks.emplace([task](){ (*task)(); });
    }

    // 唤醒一个等待的工作线程
    condition.notify_one();

    return res;
}

// 析构函数，关闭线程池
inline ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }

    // 唤醒所有等待的工作线程
    condition.notify_all();

    // 加入所有工作线程，确保它们完成剩余任务
    for(std::thread &worker: workers)
        worker.join();
}

#endif

