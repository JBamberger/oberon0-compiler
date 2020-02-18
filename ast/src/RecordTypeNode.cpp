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

#include "RecordTypeNode.h"
#include "NodeVisitor.h"
#include "PrintUtils.h"
#include <utility>

RecordTypeNode::RecordTypeNode(std::shared_ptr<Scope> parent)
    : TypeNode(), scope_(std::make_shared<Scope>("RecordScope", std::move(parent)))
{
}

RecordTypeNode::~RecordTypeNode() = default;

MemberLayout<FieldDeclarationNode>& RecordTypeNode::getMembers()
{
    return members_;
}

const MemberLayout<FieldDeclarationNode>& RecordTypeNode::getMembers() const
{
    return members_;
}

const std::shared_ptr<Scope>& RecordTypeNode::getScope() const
{
    return scope_;
}

std::string RecordTypeNode::getId() const
{
    std::stringstream s;
    s << "[R;";
    members_.visitMembers([&s](const Member<FieldDeclarationNode>& member) {
        s << member.member->getName() << "," << member.member->getType() << ";";
    });
    s << "]";
    return s.str();
}

void RecordTypeNode::visit(NodeVisitor* visitor) const
{
    visitor->visit(this);
}

void RecordTypeNode::print(std::ostream& stream) const
{
    stream << "RecordTypeNode(";
    stream << "Fields(";
    bool first = true;
    members_.visitMembers([&stream, &first](const Member<FieldDeclarationNode>& member) {
        if (first) {
            first = false;
        } else {
            stream << ", ";
        }
        stream << *member.member;
    });
    stream << "))";
}

size_t RecordTypeNode::getByteSize() const
{
    return members_.getSize();
}
