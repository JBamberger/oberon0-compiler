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

#include "ExpressionNode.h"
#include "NodeVisitor.h"

class BooleanConstantNode : public ConstantNode {

    int value_;

  public:
    BooleanConstantNode(const FilePos& pos, const int value, TypeNode* type)
        : ConstantNode(pos, type), value_(value)
    {
        assert(type->getId() == "BOOLEAN");
    }

    ~BooleanConstantNode() override = default;

    int getValue() const
    {
        return value_;
    }

    void setValue(const int value)
    {
        this->value_ = value;
    }

    void visit(NodeVisitor* visitor) const override
    {
        visitor->visit(this);
    }

    void print(std::ostream& stream) const override
    {
        stream << "BooleanConstant(" << value_ << ")";
    }
};
