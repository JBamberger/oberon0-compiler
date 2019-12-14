#include "VariableReferenceNode.h"

#include <utility>

VariableReferenceNode::VariableReferenceNode(const FilePos& pos, std::string name)
    : ExpressionNode(NodeType::variable_reference, pos), name_(std::move(name))
{
}

VariableReferenceNode::~VariableReferenceNode() = default;

void VariableReferenceNode::setSelector(const SelectorNode* node)
{
    selector_ = std::unique_ptr<const SelectorNode>(node);
}

void VariableReferenceNode::print(std::ostream& stream) const
{
    stream << "VariableReferenceNode(" << name_;
    if (selector_ != nullptr) {
        stream << ", " << *selector_;
    }
    stream << ")";
}
