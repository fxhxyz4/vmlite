#pragma once

#include <string>

// 1.0f - basic DSP
struct Config {
    float gain = 1.0f;
};

void checkGainWarnings(float percent);
Config parseArgs(int argc, char* argv[]);