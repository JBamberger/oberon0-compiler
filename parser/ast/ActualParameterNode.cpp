#include "ActualParameterNode.h"
#include "NodeVisitor.h"
#include <cassert>

ActualParameterNode::ActualParameterNode(const ExpressionNode* param)
    : Node(NodeType::parameter, param->getFilePos()), param_(param)
{
    assert(param_ != nullptr);
}

ActualParameterNode::~ActualParameterNode() = default;

const std::unique_ptr<const ExpressionNode>& ActualParameterNode::getParam() const { return param_; }

const std::unique_ptr<const ActualParameterNode>& ActualParameterNode::getNext() const { return next_; }

void ActualParameterNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

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
