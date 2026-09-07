#include "parseArgs.h"
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include "print.h"

using namespace std;

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
        } else if (arg == "--gain" || arg == "-g") {
            if (i + 1 < argc) {
                try {
                    float percent = stof(argv[++i]);
                    config.gain = percent / 100.0f;
                } catch (const exception& e) {
                    print("[ERROR] Invalid number format for " + arg);
                    exit(1);
                }
            } else {
                print("[ERROR] Missing percentage value for " + arg);
                exit(1);
            }
        } else if (arg == "-v" || arg == "--version") {
            printVersion();
            exit(0);
        }
    }

    return config;
}