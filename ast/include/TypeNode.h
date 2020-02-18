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

#pragma once
#include "Node.h"

class TypeNode : public Node {
  public:
    TypeNode() : Node(FilePos())
    {
    }

    /**
     * Get a string representation of the type.
     * @return string identifying this type
     */
    virtual std::string getId() const = 0;

    /**
     * The size of this type in memory in bytes.
     * @return size in bytes
     */
    virtual size_t getByteSize() const = 0;
};
