#include "ModuleNode.h"
#include "NodeVisitor.h"
#include "PrintUtils.h"
#include <cassert>
#include <utility>

ModuleNode::ModuleNode(const FilePos& pos, const std::string& name)
    : BlockNode(pos, name, std::make_shared<Scope>("ModuleScope " + name))
{
}

ModuleNode::~ModuleNode() = default;

BlockNode::ProcDeclList& ModuleNode::getProcedures()
{
    return procedures_;
}

const BlockNode::ProcDeclList& ModuleNode::getProcedures() const
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

void ModuleNode::visit(NodeVisitor* visitor) const
{
    visitor->visit(this);
}
