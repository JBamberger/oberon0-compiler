#include "ProcedureCallNode.h"

ProcedureCallNode::ProcedureCallNode(const FilePos& pos,
                                     std::string name,
                                     const ActualParameterNode* parameters)
    : StatementNode(NodeType::procedure_call, pos), name_(std::move(name)), parameters_(parameters)
{
    // TODO: enforce this
    // assert(parameters_ != nullptr);
}

ProcedureCallNode::~ProcedureCallNode() = default;

void ProcedureCallNode::print(std::ostream& stream) const
{
    stream << "ProcedureCall(" << name_;
    if (parameters_ != nullptr) {
        stream << ", " << *parameters_;
    }
    stream << ")";
}
