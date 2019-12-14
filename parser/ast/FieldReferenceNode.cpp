#include "FieldReferenceNode.h"
#include "NodeVisitor.h"
#include <utility>

FieldReferenceNode::FieldReferenceNode(const FilePos& pos, std::string name)
    : SelectorNode(pos), name_(std::move(name))
{
}

FieldReferenceNode::~FieldReferenceNode() = default;

const std::string& FieldReferenceNode::getName() const { return name_; }

void FieldReferenceNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void FieldReferenceNode::print(std::ostream& stream) const
{
    stream << "VariableReferenceNode(" << name_;
    if (next_ != nullptr) {
        stream << ", " << *next_;
    }
    stream << ")";
}
