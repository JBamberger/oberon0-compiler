#include "ProcedureDeclarationNode.h"
#include "NodeVisitor.h"
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

const std::unique_ptr<const ProcedureHeadingNode>& ProcedureDeclarationNode::getHeading() const
{
    return heading_;
}

const std::unique_ptr<const ProcedureBodyNode>& ProcedureDeclarationNode::getBody() const
{
    return body_;
}
void ProcedureDeclarationNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void ProcedureDeclarationNode::print(std::ostream& stream) const
{
    stream << "ProcedureDeclarationNode(" << *heading_ << ", " << *body_ << ")";
}
