#pragma once

#include "Logger.h"
#include <ostream>

enum class TokenType : char {
    eof,
    null,
    const_true,
    const_false,
    const_number,
    const_string,
    const_ident,
    period,
    comma,
    colon,
    semicolon,
    rparen,
    lparen,
    lbrack,
    rbrack,
    op_times,
    op_div,
    op_mod,
    op_plus,
    op_minus,
    op_and,
    op_or,
    op_not,
    op_eq,
    op_neq,
    op_lt,
    op_gt,
    op_leq,
    op_geq,
    op_becomes,
    kw_module,
    kw_procedure,
    kw_begin,
    kw_end,
    kw_if,
    kw_then,
    kw_else,
    kw_elsif,
    kw_while,
    kw_do,
    kw_array,
    kw_record,
    kw_const,
    kw_type,
    kw_var,
    kw_of
};

std::ostream& operator<<(std::ostream& stream, const TokenType& type);
std::string to_string(const TokenType& t);

class Token {
    TokenType type_;
    FilePos pos_;

  public:
    Token(TokenType type, FilePos pos);
    virtual ~Token();

    TokenType getType() const;
    FilePos getPosition() const;

    virtual void print(std::ostream& stream) const;
    friend std::ostream& operator<<(std::ostream& stream, const Token& symbol);
};

std::string to_string(const Token& t);
