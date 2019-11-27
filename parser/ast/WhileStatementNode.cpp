#include "WhileStatementNode.h"

WhileStatementNode::WhileStatementNode(const FilePos& pos,
                                       const ExpressionNode* condition,
                                       const StatementSequenceNode* body)
    : StatementNode(NodeType::while_statement, pos), condition_(condition), body_(body)
{
}

WhileStatementNode::~WhileStatementNode() = default;

void WhileStatementNode::print(std::ostream& stream) const
{
    stream << "While(" << *condition_ << ", " << *body_ << ")";
}
