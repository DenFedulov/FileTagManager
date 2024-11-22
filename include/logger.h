#pragma once
#include "Filesystem.h"
#include "date.h"
#include <stdexcept>

class Logger
{
private:
    std::string _logFilename;
    std::string _logHistory;
    int _errorCount = 0;

public:
    Logger(std::string logFilename);
    int getErrorCount();
    std::string makeLogEntry(std::string message, std::string data = "");
    void addLog(std::string message, std::string data = "");
    void addErrorLog(std::string message, std::string data = "");
    void writeLog();
    void dieIf(bool reason, std::string message, std::string data = "");
    void addLogIf(bool reason, std::string message, std::string data = "");
};
