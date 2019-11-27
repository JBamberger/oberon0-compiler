#include "RecordTypeNode.h"

RecordTypeNode::RecordTypeNode(const FilePos& pos) : TypeNode(NodeType::record_type, pos) {}

RecordTypeNode::~RecordTypeNode() = default;

void RecordTypeNode::addFields(const FieldListNode* fields)
{
    const auto names = fields->getNames()->getNames();
    const auto type = fields->getType();
    types_.emplace_back(type);
    for (const auto& name : names) {
        const auto fieldType = new TypeReferenceNode(type->getFilePos(), type);
        auto field = new FieldNode(fieldType->getFilePos(), name, fieldType);
        members_.emplace_back(field);
    }

    delete fields;
}

void RecordTypeNode::print(std::ostream& stream) const
{
    stream << "RecordTypeNode(Types=(";
    for (const auto& type_node : types_) stream << type_node << " ";
    stream << "), Members=(";
    for (const auto& field_node : members_) stream << field_node << " ";
    stream << "))";
}
