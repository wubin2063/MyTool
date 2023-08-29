//
// Created by wubin on 2023/8/29.
//

#ifndef WAVEGENERATOR_WAVEGENERATOR_H
#define WAVEGENERATOR_WAVEGENERATOR_H
#include <iostream>
#include <vector>

class WaveGenerator
{
public:
    void GenerateSine(uint32_t uiSampleSize, uint32_t uiCycle, double dPhase, double dAmplitude);
    void GenerateMultipleSine(uint32_t uiSampleSize, std::vector<uint32_t> vecCycle, std::vector<double> vecPhase, double dAmplitude);
};
#endif //WAVEGENERATOR_WAVEGENERATOR_H
