#include "parseArgs.h"
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include "print.h"

using namespace std;

void checkGainWarnings(float percent) {
    if (percent < 0.0f) {
        cerr << "[WARN] Negative gain (" << percent << "%) will invert the signal phase!\n";
    } else if (percent > 1000.0f) {
        cerr << "[WARN] Excessively high gain (" << percent << "%) may cause severe distortion.\n";
    }
}

Config parseArgs(int argc, char* argv[]) {
    Config config;

    if (argc <= 1) {
        print("[INFO] Please use arguments or type vmlite --help");
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
                print("[ERROR] Missing percentage value for " + arg);
                exit(1);
            }

            try {
                float percent = stof(argv[++i]);
                checkGainWarnings(percent);

                config.gain = percent / 100.0f;
            } catch (const exception& e) {
                print("[ERROR] Invalid number format for " + arg);
                exit(1);
            }
            continue;
        }

        if (!arg.empty() && arg[0] == '-') {
            print("[ERROR] Unknown option: " + arg);
            print("[INFO] Use vmlite --help to see available options.");
            exit(1);
        } else {
            print("[ERROR] Unexpected argument: " + arg);
            print("[INFO] Use vmlite --help to see available options.");
            exit(1);
        }
    }

    return config;
}