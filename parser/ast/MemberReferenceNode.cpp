#include "MemberReferenceNode.h"
#include <utility>

MemberReferenceNode::MemberReferenceNode(const FilePos& pos,
                                         std::string name,
                                         const VariableReferenceNode* next)
    : VariableReferenceNode(pos, next), name_(std::move(name))
{
}

MemberReferenceNode::~MemberReferenceNode() = default;

void MemberReferenceNode::print(std::ostream& stream) const
{
    stream << "MemberReferenceNode(" << name_ << ", " << next_ << ")";
}
