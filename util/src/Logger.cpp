/*
 * Implementation of the logger class used by the Oberon-0 compiler.
 *
 * Created by Michael Grossniklaus on 2/8/18.
 */

#include "Logger.h"
#include <iomanip>

Logger::Logger() : Logger(LogLevel::ERROR, &std::cout, &std::cerr)
{
}

Logger::Logger(LogLevel level, std::ostream* out, std::ostream* err)
    : level_(level), out_(out), err_(err)
{
}

Logger::~Logger() = default;

void Logger::log(const LogLevel level, const FilePos& filePos, const std::string& msg) const
{
    if (level < level_) {
        return;
    }

    auto out = (level == LogLevel::ERROR) ? err_ : out_;

    *out << filePos;

    switch (level) {
    case LogLevel::INFO:
        *out << "info:";
        break;
    case LogLevel::ERROR:
        *out << "error:";
        break;
    }
    *out << msg << std::endl;
}

void Logger::log(const LogLevel level, const std::string& fileName, const std::string& msg) const
{
    log(level, FilePos{fileName, -1, -1}, msg);
}

void Logger::error(const FilePos& pos, const std::string& msg) const
{
    log(LogLevel::ERROR, pos, msg);
}

void Logger::error(const std::string& fileName, const std::string& msg) const
{
    log(LogLevel::ERROR, fileName, msg);
}

void Logger::info(const std::string& fileName, const std::string& msg) const
{
    log(LogLevel::INFO, fileName, msg);
}

void Logger::setLevel(LogLevel level)
{
    level_ = level;
}
