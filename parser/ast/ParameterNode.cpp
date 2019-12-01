#include "ParameterNode.h"
#include <cassert>
#include <utility>

ParameterNode::ParameterNode(const FilePos& pos,
                             std::string name,
                             const TypeReferenceNode* type,
                             const bool is_reference)
    : TypedIdentifierNode(NodeType::parameter, pos, std::move(name), type),
      is_reference_(is_reference)
{
    assert(type_ != nullptr);
}

ParameterNode::~ParameterNode() = default;

void ParameterNode::print(std::ostream& stream) const
{
    stream << "ParameterNode(" << name_ << ", " << *type_ << ", "
           << (is_reference_ ? "ref" : "non_ref") << ")";
}
