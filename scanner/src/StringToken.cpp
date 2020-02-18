/**
 * Copyright 2020 Jannik Bamberger
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
