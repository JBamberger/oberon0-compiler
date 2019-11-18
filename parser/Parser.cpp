//
// Created by Michael Grossniklaus on 11/20/18.
//

#include <iostream>
#include "Parser.h"

Parser::Parser(Scanner *scanner, Logger *logger) :
        scanner_(scanner), logger_(logger) {
}

Parser::~Parser() = default;

const std::unique_ptr<const Node> Parser::parse() {
    return std::unique_ptr<const Node>(module());
}