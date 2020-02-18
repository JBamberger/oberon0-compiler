/*
 * Provided by Michael Grossniklaus, and modified.
 */

#pragma once

#include "Logger.h"
#include "Token.h"
#include <memory>
#include <string>
#include <unordered_map>

class Scanner {

    std::string filename_;
    const Logger* logger_;
    const Token* token_;
    int lineNo_, charNo_;
    std::unordered_map<std::string, TokenType> keywords_;
    std::unique_ptr<std::ifstream> file_in_;
    std::istream* file_;
    char ch_;

    void initTable();
    void read();
    FilePos getPosition() const;
    const Token* next();
    const Token* ident();
    int number();
    std::string string();
    void comment();

  public:
    explicit Scanner(std::istream* input, const Logger* logger);
    explicit Scanner(std::string filename, const Logger* logger);
    ~Scanner();
    const Token* peekToken();
    std::unique_ptr<const Token> nextToken();
};
