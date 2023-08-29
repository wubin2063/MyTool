//
// Created by wubin on 2023/8/29.
//

#include "WaveGenerator.h"
#include <cmath>
#include <vector>
#include <algorithm>
#include "FileTool.h"


using std::vector;

void WaveGenerator::GenerateSine(uint32_t uiSampleSize, uint32_t uiCycle, double dPhase, double dAmplitude)
{
    vector<double> vecWave(uiSampleSize);
    for (uint32_t i = 0; i < uiSampleSize; i++)
    {
        vecWave[i] = sin(2.0 * i * M_PI / ((double)uiSampleSize / uiCycle) + dPhase * (M_PI / 180.0));
        vecWave[i] *= dAmplitude;
    }
    FileTool fileTool;
    fileTool.OpenFile("./sine.txt");
    fileTool.WriteFile(vecWave);
}

// 生成多个Sine波形并保存到文件
void WaveGenerator::GenerateMultipleSine(uint32_t uiSampleSize, vector<uint32_t> vecCycle, vector<double> vecPhase, double dAmplitude)
{
    // 检查输入向量的大小是否一致
    if (vecCycle.size() != vecPhase.size())
    {
        std::cerr << "Error: vecCycle size does not match vecPhase size." << std::endl;
        return;
    }

    std::vector<double> vecResult(uiSampleSize, 0.0);

    for (size_t i = 0; i < vecCycle.size(); i++)
    {
        std::vector<double> vecWave(uiSampleSize);

        for (uint32_t j = 0; j < uiSampleSize; j++)
        {
            vecWave[j] = sin(2.0 * j * M_PI / ((double)uiSampleSize / vecCycle[i]) + vecPhase[i] * (M_PI / 180.0));
        }

        // 将当前波形叠加到结果中
        for (size_t j = 0; j < vecResult.size(); j++)
        {
            vecResult[j] += vecWave[j];
        }
    }

    // 归一化
    double dMaxAmplitude = *std::max_element(vecResult.begin(), vecResult.end());
    double dMinAmplitude = *std::min_element(vecResult.begin(), vecResult.end());
    double maxAmplitude = 0.0;

    dMaxAmplitude = std::max(fabs(dMaxAmplitude), fabs(dMinAmplitude));
    for (size_t i = 0; i < vecResult.size(); i++)
    {
        vecResult[i] *= dAmplitude / dMaxAmplitude;
    }

    FileTool fileTool;
    fileTool.OpenFile("./multiple_sine.txt");
    fileTool.WriteFile(vecResult);
}


void GenerateMultipleSine(uint32_t uiSampleSize, std::vector<uint32_t> vecCycle, std::vector<double> vecPhase, double dAmplitude)
{
    //vecCycle.size() == vecPhase.size()
    //生成多个Sine波
    //将多个Sine波叠加
    //将幅度归一化到dAmplitude
    //写入文件
}