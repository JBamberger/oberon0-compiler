#include "RecordTypeNode.h"
#include "NodeVisitor.h"
#include <utility>

RecordTypeNode::RecordTypeNode(const FilePos& pos, std::shared_ptr<Scope> parent)
    : TypeNode(NodeType::record_type, pos),
      scope_(std::make_shared<Scope>("RecordScope", std::move(parent))),
      members_(std::make_unique<std::vector<std::unique_ptr<FieldDeclarationNode>>>())
{
}

RecordTypeNode::~RecordTypeNode() = default;

std::vector<std::unique_ptr<FieldDeclarationNode>>* RecordTypeNode::getMembers() const
{
    return members_.get();
}

const std::shared_ptr<Scope>& RecordTypeNode::getScope() const { return scope_; }

std::string RecordTypeNode::getId() const
{
    std::stringstream s;
    s << "[R;";
    for (const auto& m : *members_) {
        s << m->getName() << "," << m->getType() << ";";
    }
    s << "]";
    return s.str();
}

void RecordTypeNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void RecordTypeNode::print(std::ostream& stream) const
{
    stream << "RecordTypeNode(";
    for (const auto& field_node : *members_) stream << *field_node << " ";
    stream << ")";
}