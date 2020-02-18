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
#include "TypeNode.h"

class ExpressionNode : public Node {
    TypeNode* type_;

  public:
    ExpressionNode(const FilePos& pos, TypeNode* type) : Node(pos), type_(type)
    {
    }

    TypeNode* getType() const
    {
        return type_;
    }

    void setType(TypeNode* type_node)
    {
        type_ = type_node;
    }
};

class ConstantNode : public ExpressionNode {
  public:
    ConstantNode(const FilePos& pos, TypeNode* type) : ExpressionNode(pos, type)
    {
    }
};

class AssignableExpressionNode : public ExpressionNode {
  public:
    AssignableExpressionNode(const FilePos& pos, TypeNode* type) : ExpressionNode(pos, type)
    {
    }
};
