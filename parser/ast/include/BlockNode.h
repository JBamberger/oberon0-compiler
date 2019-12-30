#pragma once
#include "Node.h"

#include "ConstantDeclarationNode.h"
#include "TypeDeclarationNode.h"

#include "Scope.h"
#include "StatementNode.h"
#include "VariableDeclarationNode.h"
#include <vector>

class ProcedureDeclarationNode;

class BlockNode : public Node {
  public:
    using ConstDeclList = std::vector<std::unique_ptr<ConstantDeclarationNode>>;
    using VarDeclList = std::vector<std::unique_ptr<VariableDeclarationNode>>;
    using TypeDeclList = std::vector<std::unique_ptr<TypeDeclarationNode>>;
    using StatementList = std::vector<std::unique_ptr<StatementNode>>;
    using ProcDeclList = std::vector<std::unique_ptr<ProcedureDeclarationNode>>;

  private:
    std::string name_;
    std::shared_ptr<Scope> scope_;
    ConstDeclList constants_;
    VarDeclList variables_;
    TypeDeclList types_;
    StatementList statements_;

  public:
    BlockNode(const FilePos& pos, std::string name, std::shared_ptr<Scope> scope);
    ~BlockNode() override;

    const std::string& getName() const;
    StatementList& getStatements();
    ConstDeclList& getConstants();
    VarDeclList& getVariables();
    TypeDeclList& getTypes();
    virtual ProcDeclList& getProcedures() = 0;

    const StatementList& getStatements() const;
    const ConstDeclList& getConstants() const;
    const VarDeclList& getVariables() const;
    const TypeDeclList& getTypes() const;
    virtual const ProcDeclList& getProcedures() const = 0;

    const std::shared_ptr<Scope>& getScope() const;
    void print(std::ostream& stream) const override;
};
