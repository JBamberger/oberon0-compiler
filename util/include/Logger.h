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

#pragma once

#include "FilePos.h"
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

enum class LogLevel : unsigned int { INFO = 2, ERROR = 3 };

/**
 * Error logger base class.
 */
class Logger {

    LogLevel level_;
    std::ostream *out_, *err_;

  protected:
    virtual void log(LogLevel level,
                     const FilePos& filePos,
                     const std::string& msg) const;
    void log(LogLevel level, const std::string& fileName, const std::string& msg) const;

  public:
    explicit Logger();
    explicit Logger(LogLevel level, std::ostream* out, std::ostream* err);
    virtual ~Logger();

    void error(const FilePos& pos, const std::string& msg) const;
    void error(const std::string& fileName, const std::string& msg) const;
    void info(const std::string& fileName, const std::string& msg) const;

    void setLevel(LogLevel level);
};

template <typename T>
static std::string to_string(T obj)
{
    std::stringstream stream;
    stream << obj;
    return stream.str();
}
