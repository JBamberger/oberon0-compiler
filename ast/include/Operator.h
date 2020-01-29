#pragma once

#include "Token.h"
#include <ostream>
#include <utility>

struct ArithmeticError : std::exception {
    std::string message;

  public:
    explicit ArithmeticError(std::string message) : exception(), message(std::move(message)) {}
};

enum class OperatorType : char { logical, arithmetic, comparison };

enum class UnaryOperator : char { plus, minus, inverse };

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
    case UnaryOperator::inverse:   return stream << "'~'";
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

inline UnaryOperator toUnaryOperator(const TokenType& type)
{
    // clang-format off
    switch (type) {
    case TokenType::op_plus:  return UnaryOperator::plus;
    case TokenType::op_minus: return UnaryOperator::minus;
    case TokenType::op_not:   return UnaryOperator::inverse;
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

inline OperatorType getOperatorType(BinaryOperator type)
{
    switch (type) {
    case BinaryOperator::eq:
    case BinaryOperator::neq:
    case BinaryOperator::lt:
    case BinaryOperator::leq:
    case BinaryOperator::gt:
    case BinaryOperator::geq:
        return OperatorType::comparison;
    case BinaryOperator::plus:
    case BinaryOperator::minus:
    case BinaryOperator::times:
    case BinaryOperator::div:
    case BinaryOperator::mod:
        return OperatorType::arithmetic;
    case BinaryOperator::logical_or:
    case BinaryOperator::logical_and:
        return OperatorType::logical;
    default:
        std::terminate();
    }
}

inline OperatorType getOperatorType(UnaryOperator type)
{
    switch (type) {
    case UnaryOperator::plus:
    case UnaryOperator::minus:
        return OperatorType ::arithmetic;
    case UnaryOperator::inverse:
        return OperatorType ::logical;
    default:
        std::terminate();
    }
}

inline int evalBinary(const BinaryOperator op, const int v1, const int v2)
{
    switch (op) {
    case BinaryOperator::times:
        return v1 * v2;
    case BinaryOperator::div:
        if (v2 == 0)
            throw ArithmeticError("Division by zero.");
        return v1 / v2;
    case BinaryOperator::mod: {
        if (v2 == 0)
            throw ArithmeticError("Division by zero.");
        const auto m = v1 % v2;
        if (m < 0) {
            return (v2 < 0) ? m - v2 : m + v2;
        }
        return m;
    }
    case BinaryOperator::plus:
        return v1 + v2;
    case BinaryOperator::minus:
        return v1 - v2;
    case BinaryOperator::logical_and:
        return v1 && v2;
    case BinaryOperator::logical_or:
        return v1 || v2;
    case BinaryOperator::eq:
        return v1 == v2;
    case BinaryOperator::neq:
        return v1 != v2;
    case BinaryOperator::lt:
        return v1 < v2;
    case BinaryOperator::leq:
        return v1 <= v2;
    case BinaryOperator::gt:
        return v1 > v2;
    case BinaryOperator::geq:
        return v1 >= v2;
    default:
        std::terminate();
    }
}

inline int evalUnary(const UnaryOperator op, const int value)
{
    switch (op) {
    case UnaryOperator::plus:
        return +value;
    case UnaryOperator::minus:
        return -value;
    case UnaryOperator::inverse:
        return !value;
    default:
        std::terminate();
    }
}