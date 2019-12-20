#include "ProcedureNode.h"
#include "NodeVisitor.h"
#include "PrintUtils.h"
#include <cassert>
#include <utility>

ProcedureNode::ProcedureNode(const FilePos& pos, std::string name, std::shared_ptr<Scope> parent)
    : BlockNode(pos, std::move(name), std::make_shared<Scope>(parent)),
      params_(std::make_unique<ParamList>()),
      procedures_(std::make_unique<std::vector<std::unique_ptr<ProcedureNode>>>())
{
}

ProcedureNode::~ProcedureNode() = default;

const std::unique_ptr<ParamList>& ProcedureNode::getParams() const { return params_; }

const std::unique_ptr<std::vector<std::unique_ptr<ProcedureNode>>>&
ProcedureNode::getProcedures() const
{
    return procedures_;
}

void ProcedureNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void ProcedureNode::print(std::ostream& stream) const
{
    stream << "ProcedureNode(";
    printList(stream, "Params", params_);
    printList(stream, "Procedures", procedures_) << ")";
    BlockNode::print(stream);
    stream << ")";
}
