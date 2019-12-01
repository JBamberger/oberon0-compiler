#include "ModuleNode.h"
#include <cassert>
#include <utility>

ModuleNode::ModuleNode(const FilePos& pos,
                       std::string name,
                       const DeclarationsNode* declarations,
                       const StatementSequenceNode* statements)
    : Node(NodeType::module, pos), name_(std::move(name)), declarations_(declarations),
      statements_(statements)
{
    assert(declarations_ != nullptr);
    assert(statements_ != nullptr);
}

ModuleNode::~ModuleNode() = default;

void ModuleNode::print(std::ostream& stream) const
{
    stream << "Module(" << name_ << ", " << *declarations_ << ", " << *statements_ << ")";
}
