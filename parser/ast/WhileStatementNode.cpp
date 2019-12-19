#include "WhileStatementNode.h"
#include "NodeVisitor.h"
#include "PrintUtils.h"
#include <cassert>

WhileStatementNode::WhileStatementNode(const FilePos& pos, const ExpressionNode* condition)
    : StatementNode(NodeType::while_statement, pos), condition_(condition),
      body_(std::make_unique<std::vector<std::unique_ptr<StatementNode>>>())
{
    assert(condition_ != nullptr);
    assert(body_ != nullptr);
}

WhileStatementNode::~WhileStatementNode() = default;

const std::unique_ptr<const ExpressionNode>& WhileStatementNode::getCondition() const
{
    return condition_;
}

const std::unique_ptr<std::vector<std::unique_ptr<StatementNode>>>&
WhileStatementNode::getBody() const
{
    return body_;
}

void WhileStatementNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void WhileStatementNode::print(std::ostream& stream) const
{
    stream << "While(" << *condition_ << ", ";
    printList(stream, "Body", body_);
    stream << ")";
}
