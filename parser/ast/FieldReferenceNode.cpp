#include "FieldReferenceNode.h"
#include <utility>

FieldReferenceNode::FieldReferenceNode(const FilePos& pos, std::string name)
    : SelectorNode(pos), name_(std::move(name))
{
}

FieldReferenceNode::~FieldReferenceNode() = default;

void FieldReferenceNode::print(std::ostream& stream) const
{
    stream << "VariableReferenceNode(" << name_;
    if (next_ != nullptr) {
        stream << ", " << *next_;
    }
    stream << ")";
}
