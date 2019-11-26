#include "ArrayReferenceNode.h"
#include <utility>

ArrayReferenceNode::ArrayReferenceNode(const FilePos& pos,
                                       const VariableReferenceNode* next,
                                       const ExpressionNode* index)
    : VariableReferenceNode(pos, next), index_(index)
{
}

ArrayReferenceNode::~ArrayReferenceNode() = default;

void ArrayReferenceNode::print(std::ostream& stream) const
{
    stream << "ArrayReferenceNode(" << index_ << ", " << next_ << ")";
}
