#include "ProcedureCallNode.h"
#include "NodeVisitor.h"
#include "PrintUtils.h"

ProcedureCallNode::ProcedureCallNode(const FilePos& pos,
                                     ProcedureDeclarationNode* procedure,
                                     ActualParamList params)
    : StatementNode(NodeType::procedure_call, pos), procedure_(procedure),
      params_(std::move(params))
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
