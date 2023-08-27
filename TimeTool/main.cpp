#include <iostream>
#include "TimerTool.h"

int main()
{
    TimerTool timer;

    // 测量程序执行时间
    timer.StartMeasure();
    // 执行需要测量时间的代码段
    timer.DelayMicroseconds(100e3); // 模拟耗时代码
    double elapsed_time = timer.StopMeasure("");

    // 延时功能
    timer.DelayMicroseconds(500);

    return 1; //conflict 9est2
    //test1
}
