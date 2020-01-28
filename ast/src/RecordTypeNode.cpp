#include "RecordTypeNode.h"
#include "NodeVisitor.h"
#include "PrintUtils.h"
#include <utility>

RecordTypeNode::RecordTypeNode(std::shared_ptr<Scope> parent)
        : TypeNode(), scope_(std::make_shared<Scope>("RecordScope", std::move(parent))) {
}

RecordTypeNode::~RecordTypeNode() = default;

MemberLayout<FieldDeclarationNode> &RecordTypeNode::getMembers() { return members_; }

const MemberLayout<FieldDeclarationNode> &RecordTypeNode::getMembers() const { return members_; }

const std::shared_ptr<Scope> &RecordTypeNode::getScope() const { return scope_; }

std::string RecordTypeNode::getId() const {
    std::stringstream s;
    s << "[R;";
    members_.visitMembers([&s](const Member<FieldDeclarationNode> &member) {
        s << member.member->getName() << "," << member.member->getType() << ";";
    });
    s << "]";
    return s.str();
}

void RecordTypeNode::visit(NodeVisitor *visitor) const { visitor->visit(this); }

void RecordTypeNode::print(std::ostream &stream) const {
    stream << "RecordTypeNode(";
    stream << "Fields(";
    bool first = true;
    members_.visitMembers([&stream, &first](const Member<FieldDeclarationNode> &member) {
        if (first) {
            first = false;
        } else {
            stream << ", ";
        }
        stream << member.member;
    });
    stream << "))";
}

size_t RecordTypeNode::getByteSize() const {
    return members_.getSize();
}
