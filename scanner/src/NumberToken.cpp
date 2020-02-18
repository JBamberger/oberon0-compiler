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
