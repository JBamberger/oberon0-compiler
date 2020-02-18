/**
 * Copyright 2020 Jannik Bamberger
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
