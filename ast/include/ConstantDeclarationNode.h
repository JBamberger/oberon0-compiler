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
#include "Node.h"

class ConstantDeclarationNode : public Node {
    std::string name_;
    std::unique_ptr<ConstantNode> value_;

  public:
    ConstantDeclarationNode(const FilePos& pos,
                            std::string name,
                            std::unique_ptr<ConstantNode> value);

    ~ConstantDeclarationNode() override;

    const std::string& getName() const;

    const std::unique_ptr<ConstantNode>& getValue() const;

    void visit(NodeVisitor* visitor) const override;

    void print(std::ostream& stream) const override;
};
