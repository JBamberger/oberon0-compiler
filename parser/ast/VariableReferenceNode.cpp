#include "VariableReferenceNode.h"

#include "NodeVisitor.h"
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

const std::string& VariableReferenceNode::getName() const { return name_; }

const std::unique_ptr<const SelectorNode>& VariableReferenceNode::getSelector() const
{
    return selector_;
}
void VariableReferenceNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void VariableReferenceNode::print(std::ostream& stream) const
{
    stream << "VariableReferenceNode(" << name_;
    if (selector_ != nullptr) {
        stream << ", " << *selector_;
    }
    stream << ")";
}
