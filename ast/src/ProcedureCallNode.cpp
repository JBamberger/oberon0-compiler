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

#include "ProcedureCallNode.h"
#include "NodeVisitor.h"
#include "PrintUtils.h"

ProcedureCallNode::ProcedureCallNode(const FilePos& pos,
                                     ProcedureDeclarationNode* procedure,
                                     ActualParamList params)
    : StatementNode(pos), procedure_(procedure), params_(std::move(params))
{
}

ProcedureCallNode::~ProcedureCallNode() = default;

ProcedureDeclarationNode* ProcedureCallNode::getProcedure() const
{
    return procedure_;
}

const std::string& ProcedureCallNode::getName() const
{
    return procedure_->getName();
}

ActualParamList& ProcedureCallNode::getParams()
{
    return params_;
}

const ActualParamList& ProcedureCallNode::getParams() const
{
    return params_;
}

void ProcedureCallNode::visit(NodeVisitor* visitor) const
{
    visitor->visit(this);
}

void ProcedureCallNode::print(std::ostream& stream) const
{
    stream << "ProcedureCall(" << getName();
    printList(stream, "Params", params_);
    stream << ")";
}
