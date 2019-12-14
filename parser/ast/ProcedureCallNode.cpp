#include "ProcedureCallNode.h"
#include "NodeVisitor.h"

ProcedureCallNode::ProcedureCallNode(const FilePos& pos,
                                     std::string name,
                                     const ActualParameterNode* parameters)
    : StatementNode(NodeType::procedure_call, pos), name_(std::move(name)), parameters_(parameters)
{
    // TODO: enforce this
    // assert(parameters_ != nullptr);
}

ProcedureCallNode::~ProcedureCallNode() = default;

const std::string& ProcedureCallNode::getName() const { return name_; }

const std::unique_ptr<const ActualParameterNode>& ProcedureCallNode::getParameters() const
{
    return parameters_;
}
void ProcedureCallNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void ProcedureCallNode::print(std::ostream& stream) const
{
    stream << "ProcedureCall(" << name_;
    if (parameters_ != nullptr) {
        stream << ", " << *parameters_;
    }
    stream << ")";
}
