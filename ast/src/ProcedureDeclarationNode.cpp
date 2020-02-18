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

#include "ProcedureDeclarationNode.h"
#include "NodeVisitor.h"
#include "PrintUtils.h"

ProcedureDeclarationNode::ProcedureDeclarationNode(const FilePos& pos,
                                                   const std::string& name,
                                                   const std::shared_ptr<Scope>& parent)
    : BlockNode(pos, name, std::make_shared<Scope>("ProcedureScope " + name, parent))
{
}

ProcedureDeclarationNode::~ProcedureDeclarationNode() = default;

ProcedureDeclarationNode::ParamDeclList& ProcedureDeclarationNode::getParams()
{
    return params_;
}

const ProcedureDeclarationNode::ParamDeclList& ProcedureDeclarationNode::getParams() const
{
    return params_;
}

const BlockNode::ProcDeclList& ProcedureDeclarationNode::getProcedures() const
{
    return procedures_;
}

BlockNode::ProcDeclList& ProcedureDeclarationNode::getProcedures()
{
    return procedures_;
}

void ProcedureDeclarationNode::visit(NodeVisitor* visitor) const
{
    visitor->visit(this);
}

void ProcedureDeclarationNode::print(std::ostream& stream) const
{
    stream << "ProcedureDeclarationNode(";
    printList(stream, "Params", params_);
    printList(stream, "Procedures", procedures_) << ")";
    BlockNode::print(stream);
    stream << ")";
}
