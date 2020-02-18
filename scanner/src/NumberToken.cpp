#include "NumberToken.h"

#include <utility>

NumberToken::NumberToken(FilePos pos, int value)
    : Token(TokenType::const_number, std::move(pos)), value_(value)
{
}

NumberToken::~NumberToken() = default;

int NumberToken::getValue() const
{
    return value_;
}

void NumberToken::print(std::ostream& stream) const
{
    stream << this->getType() << ": " << value_;
}