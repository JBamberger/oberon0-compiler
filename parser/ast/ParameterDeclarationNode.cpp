#include "ParameterDeclarationNode.h"

#include "NodeVisitor.h"
#include <cassert>
#include <utility>

ParameterDeclarationNode::ParameterDeclarationNode(const FilePos& pos,
                                                   std::string name,
                                                   std::unique_ptr<TypeReferenceNode> type,
                                                   const bool is_reference)
    : TypedIdentifierNode(NodeType::parameter, pos, std::move(name), std::move(type)),
      is_reference_(is_reference)
{
    assert(type_ != nullptr);
}

ParameterDeclarationNode::~ParameterDeclarationNode() = default;

bool ParameterDeclarationNode::isIsReference() const { return is_reference_; }
void ParameterDeclarationNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void ParameterDeclarationNode::print(std::ostream& stream) const
{
    stream << "ParameterNode(" << name_ << ", " << *type_ << ", "
           << (is_reference_ ? "ref" : "non_ref") << ")";
}
