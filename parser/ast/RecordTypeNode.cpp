#include "RecordTypeNode.h"
#include "NodeVisitor.h"
#include "PrintUtils.h"
#include <utility>

RecordTypeNode::RecordTypeNode(const FilePos& pos, std::shared_ptr<Scope> parent)
    : TypeNode(NodeType::record_type, pos),
      scope_(std::make_shared<Scope>("RecordScope", std::move(parent)))
{
}

RecordTypeNode::~RecordTypeNode() = default;

RecordTypeNode::FieldDeclList& RecordTypeNode::getMembers() { return members_; }

const RecordTypeNode::FieldDeclList& RecordTypeNode::getMembers() const { return members_; }

const std::shared_ptr<Scope>& RecordTypeNode::getScope() const { return scope_; }

std::string RecordTypeNode::getId() const
{
    std::stringstream s;
    s << "[R;";
    for (const auto& m : members_) {
        s << m->getName() << "," << m->getType() << ";";
    }
    s << "]";
    return s.str();
}

void RecordTypeNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void RecordTypeNode::print(std::ostream& stream) const
{
    stream << "RecordTypeNode(";
    printList(stream, "Fields", members_);
    stream << ")";
}