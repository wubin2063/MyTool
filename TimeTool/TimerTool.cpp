//
// Created by wubin on 2023/8/26.
//

#include "TimerTool.h"
using std::string;

/**

@brief 该函数用于开始计时，记录当前时间作为起始时间。
*/
void TimerTool::StartMeasure()
{
    start_time = std::chrono::steady_clock::now();
}

/**

@brief 该函数用于停止计时，并计算从开始计时到当前时间的时间间隔，并返回以毫秒为单位的时间间隔。
@param strName 计时记录的名称
@return double 以毫秒为单位的时间间隔
*/
double TimerTool::StopMeasure(const string &strName)
{
    auto end_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration<double, std::micro>(end_time - start_time).count();
    double milliseconds = duration / 1000.0;

    std::cout << "Time elapsed: " << milliseconds << " ms" << std::endl;
    m_mapTimeRecords[strName].push_back(milliseconds);
    start_time = std::chrono::steady_clock::now();

    return milliseconds;
}

/**

@brief 该函数用于延时指定的微秒数。
@param microseconds 延时的微秒数
*/
void TimerTool::DelayMicroseconds(double microseconds)
{
    auto start_time = std::chrono::steady_clock::now();
    while (true)
    {
        auto current_time = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(current_time - start_time).count();
        if (duration >= microseconds)
        {
            break;
        }
    }
}

/**

@brief 该函数用于获取所有的计时记录，返回一个映射关系，其中键为计时记录的名称，值为该记录的计时结果组成的向量。
@return std::map<std::string, std::vector<double>> 计时记录的映射关系
*/
std::map<std::string, std::vector<double>> TimerTool::GetTimeRecords()
{
    return m_mapTimeRecords;
}

/**

@brief 该函数用于清除所有的计时记录。
*/
void TimerTool::ClearTimeRecords()
{
    m_mapTimeRecords.clear();
}