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
#include "FieldDeclarationNode.h"

class FieldReferenceNode : public AssignableExpressionNode {

    std::unique_ptr<AssignableExpressionNode> record_ref_;
    FieldDeclarationNode* field_;

  public:
    FieldReferenceNode(const FilePos& pos,
                       FieldDeclarationNode* field,
                       std::unique_ptr<AssignableExpressionNode> record_ref);
    ~FieldReferenceNode() override;

    std::string getFieldName() const;
    FieldDeclarationNode* getField() const;
    const std::unique_ptr<AssignableExpressionNode>& getRecordRef() const;

    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
