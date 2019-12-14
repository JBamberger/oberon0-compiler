#include "AssignmentNode.h"
#include <cassert>

AssignmentNode::AssignmentNode(const FilePos& pos,
                               const VariableReferenceNode* assignee,
                               const ExpressionNode* value)
    : StatementNode(NodeType::assignment, pos), assignee_(assignee), value_(value)
{
    assert(assignee_ != nullptr);
    assert(value_ != nullptr);
}

AssignmentNode::~AssignmentNode() = default;

const std::unique_ptr<const VariableReferenceNode>& AssignmentNode::getAssignee() const
{
    return assignee_;
}

const std::unique_ptr<const ExpressionNode>& AssignmentNode::getValue() const { return value_; }

void AssignmentNode::print(std::ostream& stream) const
{
    stream << "AssignmentNode(" << *assignee_ << " := " << *value_ << ")";
}
