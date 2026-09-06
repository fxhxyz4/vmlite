#include <iostream>
#include <csignal>
#include <atomic>
#include <thread>
#include <chrono>
#include "utils/parseArgs.h"
#include "utils/print.h"
#include "modules/audio_capture.h"

using namespace std;

atomic<bool> g_running{true};

void signalHandler(int signal) {
    g_running = false;
}

int main(int argc, char* argv[]) {
    Config config = parseArgs(argc, argv);

    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    print("[INFO] Initializing VMLite audio engine...");

    if (!startAudioEngine(config.gain)) {
        print("[ERROR] Could not start audio engine.");
        return 1;
    }

    print("[INFO] VMLite is running!");
    print("[INFO] Route Spotify or other App to 'VMLite_Amplifier' in pavucontrol.");
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

    return 0;
}