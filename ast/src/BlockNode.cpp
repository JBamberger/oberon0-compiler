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

#include "BlockNode.h"

#include "PrintUtils.h"
#include <utility>

BlockNode::BlockNode(const FilePos& pos, std::string name, std::shared_ptr<Scope> scope)
    : Node(pos), name_(std::move(name)), scope_(std::move(scope))
{
}

BlockNode::~BlockNode() = default;

const std::string& BlockNode::getName() const
{
    return name_;
}

BlockNode::StatementList& BlockNode::getStatements()
{
    return statements_;
}
BlockNode::ConstDeclList& BlockNode::getConstants()
{
    return constants_;
}
MemberLayout<VariableDeclarationNode>& BlockNode::getVariables()
{
    return variables_;
}
BlockNode::TypeDeclList& BlockNode::getTypes()
{
    return types_;
}

const BlockNode::StatementList& BlockNode::getStatements() const
{
    return statements_;
}

const BlockNode::ConstDeclList& BlockNode::getConstants() const
{
    return constants_;
}

const MemberLayout<VariableDeclarationNode>& BlockNode::getVariables() const
{
    return variables_;
}

const BlockNode::TypeDeclList& BlockNode::getTypes() const
{
    return types_;
}

const std::shared_ptr<Scope>& BlockNode::getScope() const
{
    return scope_;
}

void BlockNode::print(std::ostream& stream) const
{
    stream << "BlockNode " << name_ << " (";
    printList(stream, "ConstantList", constants_) << ", ";
    stream << "VariableList(";
    bool first = true;
    variables_.visitMembers([&stream, &first](const Member<VariableDeclarationNode>& member) {
        if (first) {
            first = false;
        } else {
            stream << ", ";
        }
        stream << *member.member;
    });
    stream << ")), ";
    printList(stream, "TypeList", types_) << ", ";
    printList(stream, "StatementList", statements_) << ")";
    stream << ")";
}
