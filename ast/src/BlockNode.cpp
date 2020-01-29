#include "BlockNode.h"

#include "PrintUtils.h"
#include <utility>

BlockNode::BlockNode(const FilePos& pos, std::string name, std::shared_ptr<Scope> scope)
    : Node(NodeType::declarations, pos), name_(std::move(name)), scope_(std::move(scope))
{
}

BlockNode::~BlockNode() = default;

const std::string& BlockNode::getName() const
{
    return name_;
}

BlockNode::StatementList& BlockNode::getStatements()
{
    return statements_;
}
BlockNode::ConstDeclList& BlockNode::getConstants()
{
    return constants_;
}
MemberLayout<VariableDeclarationNode>& BlockNode::getVariables()
{
    return variables_;
}
BlockNode::TypeDeclList& BlockNode::getTypes()
{
    return types_;
}

const BlockNode::StatementList& BlockNode::getStatements() const
{
    return statements_;
}

const BlockNode::ConstDeclList& BlockNode::getConstants() const
{
    return constants_;
}

const MemberLayout<VariableDeclarationNode>& BlockNode::getVariables() const
{
    return variables_;
}

const BlockNode::TypeDeclList& BlockNode::getTypes() const
{
    return types_;
}

const std::shared_ptr<Scope>& BlockNode::getScope() const
{
    return scope_;
}

void BlockNode::print(std::ostream& stream) const
{
    stream << "BlockNode " << name_ << " (";
    printList(stream, "ConstantList", constants_) << ", ";
    stream << "VariableList(";
    bool first = true;
    variables_.visitMembers([&stream, &first](const Member<VariableDeclarationNode>& member) {
        if (first) {
            first = false;
        } else {
            stream << ", ";
        }
        stream << member.member;
    });
    stream << ")), ";
    printList(stream, "TypeList", types_) << ", ";
    printList(stream, "StatementList", statements_) << ")";
    stream << ")";
}
