#include "IfStatementNode.h"
#include "NodeVisitor.h"
#include "PrintUtils.h"
#include <cassert>

IfStatementNode::IfStatementNode(const FilePos& pos, const ExpressionNode* condition)
    : StatementNode(NodeType::if_statement, pos), condition_(condition),
      thenPart_(std::make_unique<std::vector<std::unique_ptr<StatementNode>>>()),
      elsePart_(std::make_unique<std::vector<std::unique_ptr<StatementNode>>>())
{
    assert(condition_ != nullptr);
    assert(thenPart_ != nullptr);
}

IfStatementNode::~IfStatementNode() = default;

const std::unique_ptr<const ExpressionNode>& IfStatementNode::getCondition() const
{
    return condition_;
}

const std::unique_ptr<std::vector<std::unique_ptr<StatementNode>>>&
IfStatementNode::getThenPart() const
{
    return thenPart_;
}

const std::unique_ptr<std::vector<std::unique_ptr<StatementNode>>>&
IfStatementNode::getElsePart() const
{
    return elsePart_;
}
void IfStatementNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void IfStatementNode::print(std::ostream& stream) const
{
    stream << "IfStatementNode(" << *condition_ << " then ";
    printList(stream, "ThenBody", thenPart_);
    if (elsePart_ != nullptr) {
        stream << " else ";
        printList(stream, "ElseBody", elsePart_);
    }
    stream << ")";
}
