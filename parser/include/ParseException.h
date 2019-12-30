#pragma once
#include "Logger.h"
#include <exception>

class ParseException : std::exception {
    FilePos pos_;
    std::string msg_;

  public:
    explicit ParseException(FilePos pos, std::string message)
        : exception(), pos_(std::move(pos)), msg_(std::move(message))
    {
    }
    explicit ParseException(FilePos pos, char const* message)
        : ParseException(pos, std::string(message))
    {
    }

    FilePos position() const { return pos_; }

    char const* what() const override { return msg_.c_str(); }
};
