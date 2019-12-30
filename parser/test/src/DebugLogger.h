#pragma once

#include "Logger.h"
#include <vector>

class DebugLogger : public Logger {
    std::unique_ptr<std::vector<std::string>> errors_ = std::make_unique<std::vector<std::string>>();

  protected:
    void log(LogLevel level,
             const std::string& fileName,
             int lineNo,
             int charNo,
             const std::string& msg) const override
    {
        errors_->push_back(msg);
    }

  public:
    DebugLogger() = default;
    const std::vector<std::string>& getErrors() const { return *errors_; }
};
