#include "parseArgs.h"
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include "print.h"

using namespace std;

void checkGainWarnings(float percent) {
    if (percent < 0.0f) {
        ostringstream ss;
        ss << "Negative gain (" << percent << "%) will invert the signal phase!";
        printLog(LogLevel::Warn, ss.str());
    } else if (percent > 1000.0f) {
        ostringstream ss;
        ss << "Excessively high gain (" << percent << "%) may cause severe distortion.";
        printLog(LogLevel::Warn, ss.str());
    }
}

Config parseArgs(int argc, char* argv[]) {
    Config config;

    if (argc <= 1) {
        printLog(LogLevel::Info, "Please use arguments or type vmlite --help");
        exit(1);
    }

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];

        if (arg == "--help" || arg == "-h") {
            printHelp();
            exit(0);
        }

        if (arg == "-v" || arg == "--version") {
            printVersion();
            exit(0);
        }

        if (arg == "--gain" || arg == "-g") {
            if (i + 1 >= argc) {
                printLog(LogLevel::Error, "Missing percentage value for " + arg);
                exit(1);
            }

            try {
                float percent = stof(argv[++i]);
                checkGainWarnings(percent);

                config.gain = percent / 100.0f;
            } catch (const exception& e) {
                printLog(LogLevel::Error, "Invalid number format for " + arg);
                exit(1);
            }
            continue;
        }

        if (!arg.empty() && arg[0] == '-') {
            printLog(LogLevel::Error, "Unknown option: " + arg);
            printLog(LogLevel::Info, "Use vmlite --help to see available options.");
            exit(1);
        } else {
            printLog(LogLevel::Error, "Unexpected argument: " + arg);
            printLog(LogLevel::Info, "Use vmlite --help to see available options.");
            exit(1);
        }
    }

    return config;
}