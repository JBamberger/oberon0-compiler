#include "IdentToken.h"

#include <utility>

IdentToken::IdentToken(const FilePos& pos, std::string value)
    : Token(TokenType::const_ident, pos), value_(std::move(value))
{
}

IdentToken::~IdentToken() = default;

std::string IdentToken::getValue() const
{
    return value_;
}

void IdentToken::print(std::ostream& stream) const
{
    stream << this->getType() << ": " << value_;
}