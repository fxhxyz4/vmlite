#include <iostream>
#include <cstdlib>
#include "parseArgs.h"
#include "print.h"

using namespace std;

Config parseArgs(int argc, char* argv[]) {
    Config config;

    // argv[0] executable filename
    if (argc <= 1) {
        print("[INFO] Please type a correct arguments or vmlite --help");
        exit(1);
    }

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];

        if (arg == "-v" || arg == "--version") {
            printVersion();
            return 0;
        }

        if (arg == "--help" || arg == "-h") {
            config.showHelp = true;
        } else if (arg == "--gain" || arg == "-g") {
            if (i + 1 < argc) {
                float percent = stof(argv[++i]);
                config.gain = percent / 100.0f;
            } else {
                print("[ERROR]: Missing percentage value for " + arg);
                config.showHelp = true;
            }
        }
    }

    return config;
};
