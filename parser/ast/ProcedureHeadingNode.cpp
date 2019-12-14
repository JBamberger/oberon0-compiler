#include "ProcedureHeadingNode.h"
#include "NodeVisitor.h"
#include <cassert>

ProcedureHeadingNode::ProcedureHeadingNode(const FilePos& pos,
                                           std::string name,
                                           const FormalParameterList* params)
    : Node(NodeType::procedure_heading, pos), name_(std::move(name)), params_(params)
{
    // TODO: enforce
    // assert(params_ != nullptr);
}

ProcedureHeadingNode::~ProcedureHeadingNode() = default;

const std::string& ProcedureHeadingNode::getName() const { return name_; }

const std::unique_ptr<const FormalParameterList>& ProcedureHeadingNode::getParams() const
{
    return params_;
}
void ProcedureHeadingNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void ProcedureHeadingNode::print(std::ostream& stream) const
{
    stream << "ProcedureHeadingNode(" << name_;
    if (params_ != nullptr)
        stream << ", " << *params_;
    stream << ")";
}
