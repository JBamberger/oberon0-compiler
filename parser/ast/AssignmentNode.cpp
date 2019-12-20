#include "AssignmentNode.h"
#include "NodeVisitor.h"
#include <cassert>

AssignmentNode::AssignmentNode(const FilePos& pos,
                               std::unique_ptr<VariableReferenceNode> assignee,
                               std::unique_ptr<ExpressionNode> value)
    : StatementNode(NodeType::assignment, pos), assignee_(std::move(assignee)),
      value_(std::move(value))
{
    assert(assignee_ != nullptr);
    assert(value_ != nullptr);
}

AssignmentNode::~AssignmentNode() = default;

const std::unique_ptr<VariableReferenceNode>& AssignmentNode::getAssignee() const
{
    return assignee_;
}

const std::unique_ptr<ExpressionNode>& AssignmentNode::getValue() const { return value_; }

void AssignmentNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void AssignmentNode::print(std::ostream& stream) const
{
    stream << "AssignmentNode(" << *assignee_ << " := " << *value_ << ")";
}
