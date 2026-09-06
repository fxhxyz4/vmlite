#pragma once

#include <string>

// 1.0f - basic DSP
struct Config {
    float gain = 1.0f;
    bool showHelp = false;
};

Config parseArgs(int argc, char* argv[]);