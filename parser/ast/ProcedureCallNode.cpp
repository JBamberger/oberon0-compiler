#include "ProcedureCallNode.h"
#include "NodeVisitor.h"

ProcedureCallNode::ProcedureCallNode(const FilePos& pos, std::string name)
    : StatementNode(NodeType::procedure_call, pos), name_(std::move(name)),
      parameters_(std::make_unique<std::vector<std::unique_ptr<ActualParameterNode>>>())
{
}

ProcedureCallNode::~ProcedureCallNode() = default;

const std::string& ProcedureCallNode::getName() const { return name_; }

const std::unique_ptr<std::vector<std::unique_ptr<ActualParameterNode>>>&
ProcedureCallNode::getParameters() const
{
    return parameters_;
}
void ProcedureCallNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void ProcedureCallNode::print(std::ostream& stream) const
{
    stream << "ProcedureCall(" << name_;
    for (const auto& p : *parameters_) {
        stream << ", " << *p;
    }
    stream << ")";
}
