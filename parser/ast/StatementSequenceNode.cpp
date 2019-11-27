#include "StatementSequenceNode.h"

StatementSequenceNode::StatementSequenceNode(const FilePos& pos)
    : Node(NodeType::statement_sequence, pos)
{
}

StatementSequenceNode::~StatementSequenceNode() = default;

void StatementSequenceNode::pushStatement(const StatementNode* statement)
{
    body_.emplace_back(statement);
}

void StatementSequenceNode::print(std::ostream& stream) const
{
    stream << "StatementSequence(";
    for (const auto& statement_node : body_)
    {
        stream << *statement_node << ", ";
    }
    stream << ")";
}
