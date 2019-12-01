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

void AssignmentNode::print(std::ostream& stream) const
{
    stream << "AssignmentNode(" << *assignee_ << " := " << *value_ << ")";
}
