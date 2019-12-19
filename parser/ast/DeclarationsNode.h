#pragma once

#include "ConstantDeclarationNode.h"
#include "Node.h"
#include "ProcedureDeclarationNode.h"
#include "TypeDeclarationNode.h"
#include "TypedIdentifierListNode.h"

#include <vector>

struct ProcDeclDeleter {
    void operator()(ProcedureDeclarationNode* p) { delete p; }
};

using ConstDeclList = std::vector<std::unique_ptr<ConstantDeclarationNode>>;
using VarDeclList =   std::vector<std::unique_ptr<VariableListNode>>;
using TypeDeclList =  std::vector<std::unique_ptr<TypeDeclarationNode>>;
using ProcDeclList =  std::vector<std::unique_ptr<ProcedureDeclarationNode, ProcDeclDeleter>>;

class DeclarationsNode : public Node {
    std::unique_ptr<ConstDeclList> constants_;
    std::unique_ptr<VarDeclList> variables_;
    std::unique_ptr<TypeDeclList> types_;
    std::unique_ptr<ProcDeclList> procedures_;

  public:
    DeclarationsNode(const FilePos& pos,
                     std::unique_ptr<ConstDeclList> constants,
                     std::unique_ptr<VarDeclList> vars,
                     std::unique_ptr<TypeDeclList> types,
                     std::unique_ptr<ProcDeclList> procedures);
    ~DeclarationsNode() override;


    const std::unique_ptr<ConstDeclList>& getConstants() const;
    const std::unique_ptr<VarDeclList>& getVariables() const;
    const std::unique_ptr<TypeDeclList>& getTypes() const;
    const std::unique_ptr<ProcDeclList>& getProcedures() const;

    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
