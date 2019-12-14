#include "DeclarationsNode.h"
#include "NodeVisitor.h"
#include "ProcedureDeclarationList.h"
#include <cassert>

DeclarationsNode::DeclarationsNode(const FilePos& pos,
                                   const ConstantDeclarationList* constants,
                                   const VariableDeclarationList* vars,
                                   const TypeDeclarationList* types,
                                   const ProcedureDeclarationList* procedures,
                                   void (*procDelete)(const ProcedureDeclarationList*))
    : Node(NodeType::declarations, pos), constants_(constants), variables_(vars), types_(types),
      procedures_(procedures, procDelete)
{
    assert(constants_ != nullptr);
    assert(variables_ != nullptr);
    assert(types_ != nullptr);
    assert(procedures_ != nullptr);
}

DeclarationsNode::~DeclarationsNode() = default;

const std::unique_ptr<const ConstantDeclarationList>& DeclarationsNode::getConstants() const
{
    return constants_;
}

const std::unique_ptr<const VariableDeclarationList>& DeclarationsNode::getVariables() const
{
    return variables_;
}

const std::unique_ptr<const TypeDeclarationList>& DeclarationsNode::getTypes() const
{
    return types_;
}

const std::unique_ptr<const ProcedureDeclarationList, void (*)(const ProcedureDeclarationList*)>&
DeclarationsNode::getProcedures() const
{
    return procedures_;
}



void DeclarationsNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void DeclarationsNode::print(std::ostream& stream) const
{
    stream << "Declarations(" << *constants_ << ", " << *variables_ << ", " << *types_ << ", "
           << *procedures_ << ")";
}
