#include "StatementSequenceNode.h"
#include "NodeVisitor.h"
#include <cassert>

StatementSequenceNode::StatementSequenceNode(const FilePos& pos)
    : Node(NodeType::statement_sequence, pos)
{
}

StatementSequenceNode::~StatementSequenceNode() = default;

void StatementSequenceNode::pushStatement(const StatementNode* statement)
{
    assert(statement != nullptr);

    body_.emplace_back(statement);
}

const std::vector<std::unique_ptr<const StatementNode>>& StatementSequenceNode::getBody() const
{
    return body_;
}
void StatementSequenceNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void StatementSequenceNode::print(std::ostream& stream) const
{
    stream << "StatementSequence(";
    if (!body_.empty()) {
        stream << *body_.at(0);
        for (auto i = 1; i < body_.size(); ++i) stream << ", " << *body_.at(i);
    }
    stream << ")";
}
