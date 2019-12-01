#include "ProcedureDeclarationNode.h"
#include <cassert>

ProcedureDeclarationNode::ProcedureDeclarationNode(const FilePos& pos,
                                                   const ProcedureHeadingNode* heading,
                                                   const ProcedureBodyNode* body)
    : Node(NodeType::procedure_declaration, pos), heading_(heading), body_(body)
{
    assert(heading_ != nullptr);
    assert(body_ != nullptr);
}

ProcedureDeclarationNode::~ProcedureDeclarationNode() = default;

void ProcedureDeclarationNode::print(std::ostream& stream) const
{
    stream << "ProcedureDeclarationNode(" << *heading_ << ", " << *body_ << ")";
}
