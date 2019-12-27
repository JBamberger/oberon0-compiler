#pragma once

#include "Token.h"
#include <ostream>

enum class UnaryOperator : char { plus, minus, not};

enum class BinaryOperator : char {
    plus,
    minus,
    times,
    div,
    mod,
    logical_or,
    logical_and,
    eq,
    neq,
    lt,
    leq,
    gt,
    geq
};

inline std::ostream& operator<<(std::ostream& stream, const UnaryOperator& op)
{
    // clang-format off
    switch (op) {
    case UnaryOperator::plus:  return stream << "'+'";
    case UnaryOperator::minus: return stream << "'-'";
    case UnaryOperator::not:   return stream << "'~'";
    default: std::terminate();
    }
    // clang-format on
}

inline UnaryOperator toUnaryOperator(const TokenType& type)
{
    // clang-format off
    switch (type) {
    case TokenType::op_plus:  return UnaryOperator::plus;
    case TokenType::op_minus: return UnaryOperator::minus;
    case TokenType::op_not:   return UnaryOperator::not;
    default: std::terminate();
    }
    // clang-format on
}

inline std::ostream& operator<<(std::ostream& stream, const BinaryOperator& op)
{
    // clang-format off
    switch (op) {
    case BinaryOperator::plus:        return stream << "'+'";
    case BinaryOperator::minus:       return stream << "'-'";
    case BinaryOperator::times:       return stream << "'*'";
    case BinaryOperator::div:         return stream << "'DIV'";
    case BinaryOperator::mod:         return stream << "'MOD'";
    case BinaryOperator::logical_or:  return stream << "'OR'";
    case BinaryOperator::logical_and: return stream << "'&'";
    case BinaryOperator::eq:          return stream << "'='";
    case BinaryOperator::neq:         return stream << "'#'";
    case BinaryOperator::lt:          return stream << "'<'";
    case BinaryOperator::leq:         return stream << "'<='";
    case BinaryOperator::gt:          return stream << "'>'";
    case BinaryOperator::geq:         return stream << "'>='";
    default: std::terminate();
    }
    // clang-format on
}

inline BinaryOperator toBinaryOperator(const TokenType& type)
{
    // clang-format off
    switch (type) {
    case TokenType::op_eq:    return BinaryOperator::eq;
    case TokenType::op_neq:   return BinaryOperator::neq;
    case TokenType::op_lt:    return BinaryOperator::lt;
    case TokenType::op_leq:   return BinaryOperator::leq;
    case TokenType::op_gt:    return BinaryOperator::gt;
    case TokenType::op_geq:   return BinaryOperator::geq;
    case TokenType::op_plus:  return BinaryOperator::plus;
    case TokenType::op_minus: return BinaryOperator::minus;
    case TokenType::op_or:    return BinaryOperator::logical_or;
    case TokenType::op_times: return BinaryOperator::times;
    case TokenType::op_div:   return BinaryOperator::div;
    case TokenType::op_mod:   return BinaryOperator::mod;
    case TokenType::op_and:   return BinaryOperator::logical_and;
    default: std::terminate();
    }
    // clang-format on
}