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

        printLog(LogLevel::Info, "Initializing VMLite audio engine...");

        if (!startAudioEngine(config.gain)) {
            printLog(LogLevel::Error, "Could not start audio engine.");
            return 1;
        }

        printLog(LogLevel::Info, "VMLite is running!");
        printLog(LogLevel::Info, "Route Spotify or other App to 'vmlite_sink' in pavucontrol.");
        printLog(LogLevel::Info, "Press Ctrl+C to stop...");

        while (g_running) {
            this_thread::sleep_for(chrono::milliseconds(200));
        }

        printLog(LogLevel::Info, "Stopping audio engine...");
        stopAudioEngine();
        printLog(LogLevel::Info, "VMLite closed cleanly.");

    } catch (const exception& e) {
        printLog(LogLevel::Error, string("Exception caught: ") + e.what());
        stopAudioEngine();
        return 1;
    } catch (...) {
        printLog(LogLevel::Error, "Unknown exception occurred.");
        stopAudioEngine();
        return 1;
    }

    return 0;
}