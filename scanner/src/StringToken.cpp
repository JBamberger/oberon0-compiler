#include "StringToken.h"
#include <utility>

StringToken::StringToken(const FilePos& pos, std::string value)
    : Token(TokenType::const_string, pos), value_(std::move(value))
{
}

StringToken::~StringToken() = default;

std::string StringToken::getValue() const
{
    return value_;
}

void StringToken::print(std::ostream& stream) const
{
    stream << this->getType() << ": " << value_;
}
