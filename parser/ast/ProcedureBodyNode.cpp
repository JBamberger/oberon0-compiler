#include "ProcedureBodyNode.h"
#include "NodeVisitor.h"
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

const std::string& ProcedureBodyNode::getName() const { return name_; }

const std::unique_ptr<const DeclarationsNode>& ProcedureBodyNode::getDeclarations() const
{
    return declarations_;
}

const std::unique_ptr<const StatementSequenceNode>& ProcedureBodyNode::getStatements() const
{
    return statements_;
}
void ProcedureBodyNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void ProcedureBodyNode::print(std::ostream& stream) const
{
    stream << "ProcedureBodyNode(" << name_ << ", " << *declarations_ << ", " << *statements_
           << ")";
}
