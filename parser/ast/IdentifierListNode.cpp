#include "IdentifierListNode.h"
#include "NodeVisitor.h"

IdentifierListNode::IdentifierListNode(const FilePos& pos, std::string name)
    : Node(NodeType::id_list, pos)
{
    names_.push_back(std::move(name));
}

IdentifierListNode::~IdentifierListNode() = default;

const std::vector<std::string>& IdentifierListNode::getNames() const { return names_; }

void IdentifierListNode::addName(std::string name) { names_.push_back(std::move(name)); }

void IdentifierListNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void IdentifierListNode::print(std::ostream& stream) const
{
    stream << "IdentifierListNode(";
    for (const auto& name : names_) {
        stream << name << " ";
    }
    stream << ")";
}
