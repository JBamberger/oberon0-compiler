#include "ProcedureDeclarationNode.h"
#include "NodeVisitor.h"
#include <cassert>

ProcedureDeclarationNode::ProcedureDeclarationNode(const FilePos& pos,
                                                   std::string name,
                                                   const FormalParameterList* params,
                                                   const DeclarationsNode* declarations,
                                                   const StatementSequenceNode* statements)
    : Node(NodeType::procedure_declaration, pos), name_(name), params_(params),
      declarations_(declarations), statements_(statements)
{
    assert(declarations);
    assert(statements);
}

ProcedureDeclarationNode::~ProcedureDeclarationNode() = default;

const std::string& ProcedureDeclarationNode::getName() const { return name_; }

const std::unique_ptr<const FormalParameterList>& ProcedureDeclarationNode::getParams() const
{
    return params_;
}

const std::unique_ptr<const DeclarationsNode>& ProcedureDeclarationNode::getDeclarations() const
{
    return declarations_;
}

const std::unique_ptr<const StatementSequenceNode>& ProcedureDeclarationNode::getStatements() const
{
    return statements_;
}

void ProcedureDeclarationNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void ProcedureDeclarationNode::print(std::ostream& stream) const
{
    stream << "ProcedureDeclarationNode(" << name_ << ", " << *declarations_ << ", " << *statements_
           << ")";
}
