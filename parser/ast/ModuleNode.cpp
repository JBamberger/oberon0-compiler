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

const std::string& ModuleNode::getName() const { return name_; }

const std::unique_ptr<const DeclarationsNode>& ModuleNode::getDeclarations() const
{
    return declarations_;
}

const std::unique_ptr<const StatementSequenceNode>& ModuleNode::getStatements() const
{
    return statements_;
}

void ModuleNode::print(std::ostream& stream) const
{
    stream << "Module(" << name_ << ", " << *declarations_ << ", " << *statements_ << ")";
}
