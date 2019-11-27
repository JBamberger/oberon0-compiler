#pragma once
#include "DeclarationListNode.h"
#include "Node.h"

// We need to forward-declare the ProcedureDeclarationList class because including it would result
// in a cyclic class structure. Because deleting such incomplete objects is illegal we need to use a
// custom deleter in the unique_ptr which is passed in from the outside.
class ProcedureDeclarationList;

class DeclarationsNode : public Node {
    std::unique_ptr<const ConstantDeclarationList> constants_;
    std::unique_ptr<const VariableDeclarationList> variables_;
    std::unique_ptr<const TypeDeclarationList> types_;
    std::unique_ptr<const ProcedureDeclarationList, void (*)(const ProcedureDeclarationList*)>
        procedures_;

  public:
    DeclarationsNode(const FilePos& pos,
                     const ConstantDeclarationList* constants,
                     const VariableDeclarationList* vars,
                     const TypeDeclarationList* types,
                     const ProcedureDeclarationList* procedures,
                     void (*procDelete)(const ProcedureDeclarationList*));
    ~DeclarationsNode() override;

    void print(std::ostream& stream) const override;
};
