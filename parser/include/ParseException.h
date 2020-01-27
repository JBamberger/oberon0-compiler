#pragma once
#include "Logger.h"
#include "ParserErrors.h"
#include <exception>
#include <utility>

class ParseException : public std::exception {
    FilePos pos_;
    std::string msg_;

  public:
    template <typename... Types>
    explicit ParseException(FilePos pos, const error_id id, Types&&... args)
        : exception(), pos_(std::move(pos)), msg_(getErrMsg(id, std::forward<Types>(args)...))
    {
    }

    explicit ParseException(FilePos pos, std::string message)
        : exception(), pos_(std::move(pos)), msg_(std::move(message))
    {
    }

    explicit ParseException(FilePos pos, char const* message)
        : ParseException(std::move(pos), std::string(message))
    {
    }

    FilePos position() const { return pos_; }

    char const* what() const noexcept override { return msg_.c_str(); }
};
