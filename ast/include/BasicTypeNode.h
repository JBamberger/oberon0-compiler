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
#include "TypeNode.h"

class BasicTypeNode : public TypeNode {

    std::string name_;
    size_t size_;

  public:
    BasicTypeNode(std::string name, size_t size);
    ~BasicTypeNode() override;
    const std::string& getName() const;
    std::string getId() const override;
    size_t getByteSize() const override;
    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;

    static std::unique_ptr<BasicTypeNode> makeInt();
    static std::unique_ptr<BasicTypeNode> makeBool();
    static std::unique_ptr<BasicTypeNode> makeString();
};
