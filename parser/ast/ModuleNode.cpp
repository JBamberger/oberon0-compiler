#include "ModuleNode.h"
#include "NodeVisitor.h"
#include <cassert>
#include <utility>
#include "PrintUtils.h"

ModuleNode::ModuleNode(const FilePos& pos, const std::string& name)
    : BlockNode(pos, name, std::make_shared<Scope>("ModuleScope " + name)),
      procedures_(std::make_unique<std::vector<std::unique_ptr<ProcedureDeclarationNode>>>())
{
}

ModuleNode::~ModuleNode() = default;

const std::unique_ptr<std::vector<std::unique_ptr<ProcedureDeclarationNode>>>&
ModuleNode::getProcedures() const
{
    return procedures_;
}

void ModuleNode::print(std::ostream& stream) const
{
    stream << "Module(";
    BlockNode::print(stream);
    printList(stream, "ProcedureList", procedures_) << ")";
    stream << ")";
}

void ModuleNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }
