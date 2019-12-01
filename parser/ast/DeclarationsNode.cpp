#include "DeclarationsNode.h"
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

void DeclarationsNode::print(std::ostream& stream) const
{
    stream << "Declarations(" << *constants_ << ", " << *variables_ << ", " << *types_ << ", "
           << *procedures_ << ")";
}