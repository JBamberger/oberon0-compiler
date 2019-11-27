#include "ProcedureHeadingNode.h"

ProcedureHeadingNode::ProcedureHeadingNode(const FilePos& pos,
                                           std::string name,
                                           const FormalParameterList* params)
    : Node(NodeType::procedure_heading, pos), name_(std::move(name)), params_(params)
{
}

ProcedureHeadingNode::~ProcedureHeadingNode() = default;

void ProcedureHeadingNode::print(std::ostream& stream) const
{
    stream << "ProcedureHeadingNode(" << name_;
    if (params_ != nullptr)
        stream << ", " << *params_;
    stream << ")";
}
