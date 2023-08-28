#include <iostream>
#include "ThreadPool.h"

// 一个示例函数，计算给定数字的平方
int square(const int &x) {
    return x * x;
}

int main() {
    // 创建线程池，使用4个工作线程
    ThreadPool pool(4);
    double d = square(2);
    std::cout  << d << std::endl;
    // 创建一个 vector 以保存任务的结果
    std::vector<std::future<int>> results;

    // 向线程池添加任务
    for (int i = 1; i <= 10; ++i) {
        results.emplace_back(pool.enqueue(square, i));
    }

    // 等待所有任务完成并获取结果
    for (auto& result : results) {
        std::cout << "Result: " << result.get() << std::endl;
    }

    // 线程池会在作用域结束时自动销毁

    return 0;
}

