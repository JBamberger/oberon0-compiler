#include "ProcedureCallNode.h"
#include "NodeVisitor.h"

ProcedureCallNode::ProcedureCallNode(
    const FilePos& pos,
    ProcedureDeclarationNode* procedure,
    std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>> params)
    : StatementNode(NodeType::procedure_call, pos), procedure_(procedure),
      params_(std::move(params))
{
}

ProcedureCallNode::~ProcedureCallNode() = default;

ProcedureDeclarationNode* ProcedureCallNode::getProcedure() const { return procedure_; }

const std::string& ProcedureCallNode::getName() const { return procedure_->getName(); }

const std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>>&
ProcedureCallNode::getParams() const
{
    return params_;
}
void ProcedureCallNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void ProcedureCallNode::print(std::ostream& stream) const
{
    stream << "ProcedureCall(" << getName();
    for (const auto& p : *params_) {
        stream << ", " << *p;
    }
    stream << ")";
}
