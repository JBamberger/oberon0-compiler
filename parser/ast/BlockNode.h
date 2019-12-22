#pragma once
#include "Node.h"

#include "ConstantDeclarationNode.h"
#include "TypeDeclarationNode.h"
#include "TypedIdentifierListNode.h"

#include "StatementNode.h"
#include <vector>
#include "Scope.h"

using ConstDeclList = std::vector<std::unique_ptr<ConstantDeclarationNode>>;
using VarDeclList = std::vector<std::unique_ptr<VariableDeclarationNode>>;
using TypeDeclList = std::vector<std::unique_ptr<TypeDeclarationNode>>;
using StatementList = std::vector<std::unique_ptr<StatementNode>>;

class ProcedureDeclarationNode;

class BlockNode : public Node {
    std::string name_;
    std::shared_ptr<Scope> scope_;
    std::unique_ptr<ConstDeclList> constants_;
    std::unique_ptr<VarDeclList> variables_;
    std::unique_ptr<TypeDeclList> types_;
    std::unique_ptr<StatementList> statements_;

  public:
    BlockNode(const FilePos& pos, std::string name, std::shared_ptr<Scope> scope);
    ~BlockNode() override;

    const std::string& getName() const;
    const std::unique_ptr<StatementList>& getStatements() const;
    const std::unique_ptr<ConstDeclList>& getConstants() const;
    const std::unique_ptr<VarDeclList>& getVariables() const;
    const std::unique_ptr<TypeDeclList>& getTypes() const;
    virtual const std::unique_ptr<std::vector<std::unique_ptr<ProcedureDeclarationNode>>>&
    getProcedures() const = 0;


    const std::shared_ptr<Scope>& getScope() const;
    void print(std::ostream& stream) const override;
};
