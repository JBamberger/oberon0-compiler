#pragma once
#include "Token.h"

class StringToken final : public Token {

  private:
    std::string value_;

  public:
    explicit StringToken(const FilePos& pos, std::string  value);
    ~StringToken() override;

    std::string getValue() const;

    void print(std::ostream& stream) const override;
};
