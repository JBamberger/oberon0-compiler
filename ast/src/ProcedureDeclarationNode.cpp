#include "ProcedureDeclarationNode.h"
#include "NodeVisitor.h"
#include "PrintUtils.h"

ProcedureDeclarationNode::ProcedureDeclarationNode(const FilePos& pos,
                                                   const std::string& name,
                                                   const std::shared_ptr<Scope>& parent)
    : BlockNode(pos, name, std::make_shared<Scope>("ProcedureScope " + name, parent))
{
}

ProcedureDeclarationNode::~ProcedureDeclarationNode() = default;

ProcedureDeclarationNode::ParamDeclList& ProcedureDeclarationNode::getParams() { return params_; }

const ProcedureDeclarationNode::ParamDeclList& ProcedureDeclarationNode::getParams() const
{
    return params_;
}

const BlockNode::ProcDeclList& ProcedureDeclarationNode::getProcedures() const
{
    return procedures_;
}

BlockNode::ProcDeclList& ProcedureDeclarationNode::getProcedures() { return procedures_; }

void ProcedureDeclarationNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void ProcedureDeclarationNode::print(std::ostream& stream) const
{
    stream << "ProcedureDeclarationNode(";
    printList(stream, "Params", params_);
    printList(stream, "Procedures", procedures_) << ")";
    BlockNode::print(stream);
    stream << ")";
}
