#include "logger.h"

Logger::Logger(std::string logFilename)
{
    this->logFilename = logFilename;
}

int Logger::getErrorCount()
{
    return this->errorCount;
}

std::string Logger::makeLogEntry(std::string message, std::string data)
{
    std::string stringToWrite = getCurrentDate() + " " + message + "\n";
    if (!data.empty())
    {
        stringToWrite += data + "\n";
    }
    return stringToWrite;
}

void Logger::addLog(std::string message, std::string data)
{
    this->logHistory += this->makeLogEntry(message, data);
}

void Logger::addErrorLog(std::string message, std::string data)
{
    this->errorCount++;
    this->addLog(message, data);
}

void Logger::writeLog()
{
    fileWrite(this->logFilename, this->logHistory, true);
}

void Logger::dieIf(bool reason, std::string message, std::string data)
{
    if (reason)
    {
        this->addErrorLog(message, data);
        this->writeLog();
        throw std::runtime_error(message);
    }
}

void Logger::addLogIf(bool reason, std::string message, std::string data)
{
    if (reason)
    {
        this->addLog(message, data);
    }
}