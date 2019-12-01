#include "ActualParameterNode.h"
#include <cassert>

ActualParameterNode::ActualParameterNode(const ExpressionNode* param)
    : Node(NodeType::parameter, param->getFilePos()), param_(param)
{
    assert(param_ != nullptr);
}

ActualParameterNode::~ActualParameterNode() = default;

void ActualParameterNode::setNext(const ActualParameterNode* next)
{
    next_ = std::unique_ptr<const ActualParameterNode>(next);
}

void ActualParameterNode::print(std::ostream& stream) const
{
    stream << "ActualParameterNode(" << *param_;
    if (next_ != nullptr) {
        stream << ", " << *next_;
    }
    stream << ")";
}
