#include "VariableReferenceNode.h"
#include <utility>

VariableReferenceNode::VariableReferenceNode(const FilePos& pos, std::string name)
    : ChainedReferenceNode(pos), name_(std::move(name))
{
}

VariableReferenceNode::~VariableReferenceNode() = default;

void VariableReferenceNode::print(std::ostream& stream) const
{
    stream << "MemberReferenceNode(" << name_;
    if (next_ != nullptr) {
        stream << ", " << *next_;
    }
    stream << ")";
}
