#pragma once

#include <string>

using namespace std;

enum class LogLevel {
    Info,
    Warn,
    Error
};

void printHelp();
void printBanner();
void printVersion();
void printLog(LogLevel level, const string& message);