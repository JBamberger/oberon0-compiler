#include "VariableReferenceNode.h"

#include "NodeVisitor.h"
#include <utility>

VariableReferenceNode::VariableReferenceNode(const FilePos& pos, std::string name)
    : ExpressionNode(NodeType::variable_reference, pos), name_(std::move(name))
{
}

VariableReferenceNode::VariableReferenceNode(const FilePos& pos,
                                             std::string name,
                                             std::unique_ptr<SelectorNode> selector)
    : ExpressionNode(NodeType::variable_reference, pos), name_(std::move(name)),
      selector_(std::move(selector))
{
}

VariableReferenceNode::~VariableReferenceNode() = default;

void VariableReferenceNode::setSelector(std::unique_ptr<SelectorNode> node)
{
    selector_ = std::move(node);
}

const std::string& VariableReferenceNode::getName() const { return name_; }

const std::unique_ptr<SelectorNode>& VariableReferenceNode::getSelector() const
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
