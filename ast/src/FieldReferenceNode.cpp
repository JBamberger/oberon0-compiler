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

#include "FieldReferenceNode.h"
#include "NodeVisitor.h"
#include <utility>

FieldReferenceNode::FieldReferenceNode(const FilePos& pos,
                                       FieldDeclarationNode* field,
                                       std::unique_ptr<AssignableExpressionNode> record_ref)
    : AssignableExpressionNode(pos, field->getType()), record_ref_(std::move(record_ref)),
      field_(field)
{
}

const std::unique_ptr<AssignableExpressionNode>& FieldReferenceNode::getRecordRef() const
{
    return record_ref_;
}

FieldReferenceNode::~FieldReferenceNode() = default;

std::string FieldReferenceNode::getFieldName() const
{
    return field_->getName();
}

FieldDeclarationNode* FieldReferenceNode::getField() const
{
    return field_;
}

void FieldReferenceNode::visit(NodeVisitor* visitor) const
{
    visitor->visit(this);
}

void FieldReferenceNode::print(std::ostream& stream) const
{
    stream << "VariableReferenceNode(" << *record_ref_ << ", " << field_->getName() << ")";
}
