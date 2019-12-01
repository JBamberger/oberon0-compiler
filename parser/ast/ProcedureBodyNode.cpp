#include "ProcedureBodyNode.h"
#include <cassert>

ProcedureBodyNode::ProcedureBodyNode(const FilePos& pos,
                                     std::string name,
                                     const DeclarationsNode* declarations,
                                     const StatementSequenceNode* statements)
    : Node(NodeType::procedure_body, pos), name_(std::move(name)), declarations_(declarations),
      statements_(statements)
{
    assert(declarations_ != nullptr);
    assert(statements_ != nullptr);
}

ProcedureBodyNode::~ProcedureBodyNode() = default;

void ProcedureBodyNode::print(std::ostream& stream) const
{
    stream << "ProcedureBodyNode(" << name_ << ", " << *declarations_ << ", " << *statements_
           << ")";
}
