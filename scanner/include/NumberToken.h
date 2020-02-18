#pragma once

#include "Token.h"

class NumberToken final : public Token {

    int value_;

  public:
    NumberToken(FilePos pos, int value);
    ~NumberToken() override;

    int getValue() const;

    void print(std::ostream& stream) const override;
};
