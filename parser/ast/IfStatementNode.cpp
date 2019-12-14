#include "IfStatementNode.h"
#include <cassert>

IfStatementNode::IfStatementNode(const FilePos& pos,
                                 const ExpressionNode* condition,
                                 const StatementSequenceNode* thenPart)
    : StatementNode(NodeType::if_statement, pos), condition_(condition), thenPart_(thenPart)
{
    assert(condition_ != nullptr);
    assert(thenPart_ != nullptr);
}

IfStatementNode::~IfStatementNode() = default;

void IfStatementNode::setElseBody(const StatementSequenceNode* elsePart)
{
    assert(elsePart != nullptr);

    elsePart_ = std::unique_ptr<const StatementSequenceNode>(elsePart);
}

const std::unique_ptr<const ExpressionNode>& IfStatementNode::getCondition() const
{
    return condition_;
}

const std::unique_ptr<const StatementSequenceNode>& IfStatementNode::getThenPart() const
{
    return thenPart_;
}

const std::unique_ptr<const StatementSequenceNode>& IfStatementNode::getElsePart() const
{
    return elsePart_;
}

void IfStatementNode::print(std::ostream& stream) const
{
    stream << "IfStatementNode(" << *condition_ << " then " << *thenPart_;
    if (elsePart_ != nullptr) {
        stream << " else " << *elsePart_;
    }
    stream << ")";
}
