#include "WhileStatementNode.h"
#include <cassert>

WhileStatementNode::WhileStatementNode(const FilePos& pos,
                                       const ExpressionNode* condition,
                                       const StatementSequenceNode* body)
    : StatementNode(NodeType::while_statement, pos), condition_(condition), body_(body)
{
    assert(condition_ != nullptr);
    assert(body_ != nullptr);
}

WhileStatementNode::~WhileStatementNode() = default;

void WhileStatementNode::print(std::ostream& stream) const
{
    stream << "While(" << *condition_ << ", " << *body_ << ")";
}
