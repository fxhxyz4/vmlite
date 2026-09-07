#include "modules/audio_capture.h"
#include "utils/parseArgs.h"
#include "utils/print.h"
#include <stdexcept>
#include <iostream>
#include <csignal>
#include <atomic>
#include <thread>
#include <chrono>

using namespace std;

atomic<bool> g_running{true};

void signalHandler(int signal) {
    (void)signal;
    g_running = false;
}

int main(int argc, char* argv[]) {
    try {
        printBanner();
        Config config = parseArgs(argc, argv);

        signal(SIGINT, signalHandler);
        signal(SIGTERM, signalHandler);

        print("[INFO] Initializing VMLite audio engine...");

        if (!startAudioEngine(config.gain)) {
            print("[ERROR] Could not start audio engine.");
            return 1;
        }

        print("[INFO] VMLite is running!");
        print("[INFO] Route Spotify or other App to 'vmlite_sink' in pavucontrol.");
        print("[INFO] Press ENTER or Ctrl+C to stop...");

        while (g_running) {
            this_thread::sleep_for(chrono::milliseconds(100));
            if (cin.rdbuf()->in_avail() > 0) {
                break;
            }
        }

        print("[INFO] Stopping audio engine...");
        stopAudioEngine();
        print("[INFO] VMLite closed cleanly.");

    } catch (const exception& e) {
        print(string("[ERROR] Exception caught: ") + e.what());
        stopAudioEngine();
        return 1;
    } catch (...) {
        print("[ERROR] Unknown exception occurred.");
        stopAudioEngine();
        return 1;
    }

    return 0;
}