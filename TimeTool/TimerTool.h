//
// Created by wubin on 2023/8/26.
//

#ifndef TIMETOOL_TIMERTOOL_H
#define TIMETOOL_TIMERTOOL_H

#include <iostream>
#include <chrono>
#include <string>
#include <map>
#include <vector>

class TimerTool
{
public:
    void StartMeasure();
    double StopMeasure(const std::string &strName = "");
    void DelayMicroseconds(double microseconds);
    //获得时间记录统计
    std::map<std::string, std::vector<double>> GetTimeRecords();
    //清空时间记录
    void ClearTimeRecords();

private:
    std::chrono::time_point<std::chrono::steady_clock> start_time;
    std::map<std::string, std::vector<double>> m_mapTimeRecords;
};


#endif //TIMETOOL_TIMERTOOL_H
