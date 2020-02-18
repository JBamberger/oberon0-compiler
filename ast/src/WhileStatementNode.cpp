#include "WhileStatementNode.h"
#include "NodeVisitor.h"
#include "PrintUtils.h"
#include <cassert>

WhileStatementNode::WhileStatementNode(const FilePos& pos,
                                       std::unique_ptr<ExpressionNode> condition)
    : StatementNode(pos), condition_(std::move(condition))
{
    assert(condition_ != nullptr);
}

WhileStatementNode::~WhileStatementNode() = default;

const std::unique_ptr<ExpressionNode>& WhileStatementNode::getCondition() const
{
    return condition_;
}

WhileStatementNode::StmtList& WhileStatementNode::getBody()
{
    return body_;
}

const WhileStatementNode::StmtList& WhileStatementNode::getBody() const
{
    return body_;
}

void WhileStatementNode::visit(NodeVisitor* visitor) const
{
    visitor->visit(this);
}

void WhileStatementNode::print(std::ostream& stream) const
{
    stream << "While(" << *condition_ << ", ";
    printList(stream, "Body", body_);
    stream << ")";
}
