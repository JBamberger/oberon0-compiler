#include "ProcedureCallNode.h"

ProcedureCallNode::ProcedureCallNode(const VariableReferenceNode* name,
                                     const ActualParameterNode* parameters)
    : StatementNode(NodeType::procedure_call, name->getFilePos()), name_(name),
      parameters_(parameters)
{
}

ProcedureCallNode::~ProcedureCallNode() = default;

void ProcedureCallNode::print(std::ostream& stream) const
{
    stream << "ProcedureCall(" << *name_;
    if (parameters_ != nullptr) {
        stream << ", " << *parameters_;
    }
    stream << ")";
}
