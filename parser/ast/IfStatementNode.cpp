#include "IfStatementNode.h"
#include "NodeVisitor.h"
#include "PrintUtils.h"
#include <cassert>

IfStatementNode::IfStatementNode(const FilePos& pos, std::unique_ptr<ExpressionNode> condition)
    : StatementNode(NodeType::if_statement, pos), condition_(std::move(condition))
{
    assert(condition_ != nullptr);
}

IfStatementNode::~IfStatementNode() = default;

const std::unique_ptr<ExpressionNode>& IfStatementNode::getCondition() const { return condition_; }

IfStatementNode::StmtList& IfStatementNode::getThenPart() { return thenPart_; }

IfStatementNode::StmtList& IfStatementNode::getElsePart() { return elsePart_; }

const IfStatementNode::StmtList& IfStatementNode::getThenPart() const { return thenPart_; }

const IfStatementNode::StmtList& IfStatementNode::getElsePart() const { return elsePart_; }

void IfStatementNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void IfStatementNode::print(std::ostream& stream) const
{
    stream << "IfStatementNode(" << *condition_ << " then ";
    printList(stream, "ThenBody", thenPart_);
    if (!elsePart_.empty()) {
        stream << " else ";
        printList(stream, "ElseBody", elsePart_);
    }
    stream << ")";
}
