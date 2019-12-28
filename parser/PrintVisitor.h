#pragma once
#include "ast/NodeVisitor.h"
#include <ostream>

class PrintVisitor : public NodeVisitor {
    std::ostream& out_;
    int indent_;

    std::ostream& line();
    void inc();
    void dec();

  public:
    explicit PrintVisitor(std::ostream& out);
    ~PrintVisitor() override;
    void visit(const ArrayReferenceNode* node) override;
    void visit(const ArrayTypeNode* node) override;
    void visit(const AssignmentNode* node) override;
    void visit(const BasicTypeNode* node) override;
    void visit(const BinaryExpressionNode* node) override;
    void visit(const FieldReferenceNode* node) override;
    void visit(const ConstantDeclarationNode* node) override;
    void visit(const FieldDeclarationNode* node) override;
    void visit(const IfStatementNode* node) override;
    void visit(const ModuleNode* node) override;
    void visit(const NumberConstantNode* node) override;
    void visit(const ParameterDeclarationNode* node) override;
    void visit(const ProcedureCallNode* node) override;
    void visit(const ProcedureDeclarationNode* node) override;
    void visit(const RecordTypeNode* node) override;
    void visit(const StringConstantNode* node) override;
    void visit(const TypeDeclarationNode* node) override;
    void visit(const TypedIdentifierNode* node) override;
    void visit(const UnaryExpressionNode* node) override;
    void visit(const VariableDeclarationNode* node) override;
    void visit(const VariableReferenceNode* node) override;
    void visit(const ParameterReferenceNode* node) override;
    void visit(const WhileStatementNode* node) override;
    void visit(const BooleanConstantNode* node) override;
private:
    void printBlock(const BlockNode* node);
};
