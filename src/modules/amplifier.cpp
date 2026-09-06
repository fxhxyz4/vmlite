#include <iostream>
#include <algorithm>
#include "amplifier.h"

using namespace std;

void amplifyBuffer(float* buffer, size_t sampleCount, float gain) {
    for (int i = 0; i < sampleCount; i++) {
        float amplifiedSample = buffer[i] *= gain;
        buffer[i] = clamp(amplifiedSample, -1.0f, 1.0f);
    }
}
