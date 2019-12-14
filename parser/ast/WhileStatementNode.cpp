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

const std::unique_ptr<const ExpressionNode>& WhileStatementNode::getCondition() const
{
    return condition_;
}

const std::unique_ptr<const StatementSequenceNode>& WhileStatementNode::getBody() const
{
    return body_;
}

void WhileStatementNode::print(std::ostream& stream) const
{
    stream << "While(" << *condition_ << ", " << *body_ << ")";
}
