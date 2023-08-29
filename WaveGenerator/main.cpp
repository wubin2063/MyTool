#include <iostream>
#include "WaveGenerator.h"
int main()
{
    WaveGenerator waveGenerator;
    waveGenerator.GenerateSine(1000,17, 10.0, 2.2);
    waveGenerator.GenerateMultipleSine(1000, {12,14,16},{5.7,55,90}, 3.3);
    return 0;
}
