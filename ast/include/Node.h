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

#include "Logger.h"
#include <memory>
#include <ostream>

class NodeVisitor;

/**
 * Base class for all abstract syntax tree nodes.
 */
class Node {
    FilePos pos_;

  public:
    explicit Node(FilePos pos);

    virtual ~Node() = 0;

    FilePos getFilePos() const;

    virtual void visit(NodeVisitor* visitor) const = 0;

    virtual void print(std::ostream& stream) const = 0;

    friend std::ostream& operator<<(std::ostream& stream, const Node& node);
};
