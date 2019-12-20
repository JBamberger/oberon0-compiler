#include "BlockNode.h"

#include "NodeVisitor.h"
#include "PrintUtils.h"
#include <utility>

BlockNode::BlockNode(const FilePos& pos, std::string name, std::shared_ptr<Scope> scope)
    : Node(NodeType::declarations, pos), name_(std::move(name)), scope_(std::move(scope)),
      constants_(std::make_unique<ConstDeclList>()), variables_(std::make_unique<VarDeclList>()),
      types_(std::make_unique<TypeDeclList>()), statements_(std::make_unique<StatementList>())
{
}

BlockNode::~BlockNode() = default;

const std::string& BlockNode::getName() const { return name_; }

const std::unique_ptr<StatementList>& BlockNode::getStatements() const { return statements_; }

const std::unique_ptr<ConstDeclList>& BlockNode::getConstants() const { return constants_; }

const std::unique_ptr<VarDeclList>& BlockNode::getVariables() const { return variables_; }

const std::unique_ptr<TypeDeclList>& BlockNode::getTypes() const { return types_; }

const std::shared_ptr<Scope>& BlockNode::getScope() const { return scope_; }

void BlockNode::print(std::ostream& stream) const
{
    stream << "BlockNode " << name_ << " (";
    printList(stream, "ConstantList", constants_) << ", ";
    printList(stream, "VariableList", variables_) << ", ";
    printList(stream, "TypeList", types_) << ", ";
    printList(stream, "StatementList", statements_) << ")";
    stream << ")";
}
