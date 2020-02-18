#pragma once
#include "Token.h"

class IdentToken final : public Token {

    std::string value_;

  public:
    explicit IdentToken(const FilePos& pos, std::string  value);

    ~IdentToken() override;

    std::string getValue() const;

    void print(std::ostream& stream) const override;
};
