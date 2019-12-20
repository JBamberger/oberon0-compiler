#include "ActualParameterNode.h"
#include "NodeVisitor.h"
#include <cassert>

ActualParameterNode::ActualParameterNode(std::unique_ptr<ExpressionNode> param)
    : Node(NodeType::parameter, param->getFilePos()), param_(std::move(param))
{
    assert(param_ != nullptr);
}

ActualParameterNode::~ActualParameterNode() = default;

const std::unique_ptr<ExpressionNode>& ActualParameterNode::getParam() const { return param_; }

void ActualParameterNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void ActualParameterNode::print(std::ostream& stream) const
{
    stream << "ActualParameterNode(" << *param_ << ")";
}
