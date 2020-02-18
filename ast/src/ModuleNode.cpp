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

#include "ModuleNode.h"
#include "NodeVisitor.h"
#include "PrintUtils.h"
#include <cassert>
#include <utility>

ModuleNode::ModuleNode(const FilePos& pos, const std::string& name)
    : BlockNode(pos, name, std::make_shared<Scope>("ModuleScope " + name))
{
}

ModuleNode::~ModuleNode() = default;

BlockNode::ProcDeclList& ModuleNode::getProcedures()
{
    return procedures_;
}

const BlockNode::ProcDeclList& ModuleNode::getProcedures() const
{
    return procedures_;
}

void ModuleNode::print(std::ostream& stream) const
{
    stream << "Module(";
    BlockNode::print(stream);
    printList(stream, "ProcedureList", procedures_) << ")";
    stream << ")";
}

void ModuleNode::visit(NodeVisitor* visitor) const
{
    visitor->visit(this);
}
