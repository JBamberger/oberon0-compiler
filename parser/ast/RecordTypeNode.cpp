#include "RecordTypeNode.h"
#include "NodeVisitor.h"
#include <cassert>

RecordTypeNode::RecordTypeNode(const FilePos& pos) : TypeNode(NodeType::record_type, pos) {}

RecordTypeNode::~RecordTypeNode() = default;

void RecordTypeNode::addFields(const FieldListNode* fields)
{
    assert(fields != nullptr);

    const auto pairs = fields->getPairs();
    const auto type = fields->getType();
    types_.emplace_back(type);

    for (auto& pair : *pairs) members_.push_back(std::move(pair));

    delete fields;
}

const std::vector<std::shared_ptr<const TypeNode>>& RecordTypeNode::getTypes() const
{
    return types_;
}

const std::vector<std::unique_ptr<const FieldNode>>& RecordTypeNode::getMembers() const
{
    return members_;
}
void RecordTypeNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void RecordTypeNode::print(std::ostream& stream) const
{
    stream << "RecordTypeNode(Types=(";
    for (const auto& type_node : types_) stream << *type_node << " ";
    stream << "), Members=(";
    for (const auto& field_node : members_) stream << *field_node << " ";
    stream << "))";
}
