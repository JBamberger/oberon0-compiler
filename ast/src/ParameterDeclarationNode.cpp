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

#include "ParameterDeclarationNode.h"

#include "NodeVisitor.h"
#include <utility>

ParameterDeclarationNode::ParameterDeclarationNode(const FilePos& pos,
                                                   std::string name,
                                                   TypeNode* type,
                                                   const bool is_reference)
    : TypedIdentifierNode(pos, std::move(name), type), is_reference_(is_reference)
{
}

ParameterDeclarationNode::~ParameterDeclarationNode() = default;

bool ParameterDeclarationNode::isIsReference() const
{
    return is_reference_;
}

void ParameterDeclarationNode::visit(NodeVisitor* visitor) const
{
    visitor->visit(this);
}

void ParameterDeclarationNode::print(std::ostream& stream) const
{
    stream << "ParameterNode(" << name_ << ", " << type_ << ", "
           << (is_reference_ ? "ref" : "non_ref") << ")";
}
