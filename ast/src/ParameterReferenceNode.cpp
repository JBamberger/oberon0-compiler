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

#include "ParameterReferenceNode.h"

#include "NodeVisitor.h"
#include <utility>

ParameterReferenceNode::ParameterReferenceNode(const FilePos& pos, ParameterDeclarationNode* param)
    : AssignableExpressionNode(pos, param->getType()), param_(param)
{
}

ParameterReferenceNode::~ParameterReferenceNode() = default;

std::string ParameterReferenceNode::getName() const
{
    return param_->getName();
}

ParameterDeclarationNode* ParameterReferenceNode::getParam() const
{
    return param_;
}

void ParameterReferenceNode::visit(NodeVisitor* visitor) const
{
    visitor->visit(this);
}

void ParameterReferenceNode::print(std::ostream& stream) const
{
    stream << "ParameterReferenceNode(" << param_->getType() << ")";
}
