#include "ProcedureDeclarationNode.h"
#include "NodeVisitor.h"
#include "PrintUtils.h"
#include <cassert>
#include <utility>

ProcedureDeclarationNode::ProcedureDeclarationNode(const FilePos& pos,
                                                   std::string name,
                                                   std::shared_ptr<Scope> parent)
    : BlockNode(pos, std::move(name), std::make_shared<Scope>(parent)),
      params_(std::make_unique<ParamList>()),
      procedures_(std::make_unique<std::vector<std::unique_ptr<ProcedureDeclarationNode>>>())
{
}

ProcedureDeclarationNode::~ProcedureDeclarationNode() = default;

const std::unique_ptr<ParamList>& ProcedureDeclarationNode::getParams() const { return params_; }

const std::unique_ptr<std::vector<std::unique_ptr<ProcedureDeclarationNode>>>&
ProcedureDeclarationNode::getProcedures() const
{
    return procedures_;
}

void ProcedureDeclarationNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void ProcedureDeclarationNode::print(std::ostream& stream) const
{
    stream << "ProcedureDeclarationNode(";
    printList(stream, "Params", params_);
    printList(stream, "Procedures", procedures_) << ")";
    BlockNode::print(stream);
    stream << ")";
}
