#pragma once
#include "NodeVisitor.h"
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
    void visit(const ActualParameterNode* node) override;
    void visit(const ArrayReferenceNode* node) override;
    void visit(const ArrayTypeNode* node) override;
    void visit(const AssignmentNode* node) override;
    void visit(const BasicTypeNode* node) override;
    void visit(const BinaryExpressionNode* node) override;
    void visit(const FieldReferenceNode* node) override;
    void visit(const ConstantDeclarationNode* node) override;
    void visit(const ConstantDeclarationList* node) override;
    void visit(const VariableDeclarationList* node) override;
    void visit(const TypeDeclarationList* node) override;
    void visit(const FormalParameterList* node) override;
    void visit(const DeclarationsNode* node) override;
    void visit(const FieldNode* node) override;
    void visit(const IdentifierListNode* node) override;
    void visit(const IfStatementNode* node) override;
    void visit(const ModuleNode* node) override;
    void visit(const NumberConstantNode* node) override;
    void visit(const ParameterNode* node) override;
    void visit(const ProcedureCallNode* node) override;
    void visit(const ProcedureDeclarationNode* node) override;
    void visit(const RecordTypeNode* node) override;
    void visit(const StatementSequenceNode* node) override;
    void visit(const StringConstantNode* node) override;
    void visit(const TypeDeclarationNode* node) override;
    void visit(const FieldListNode* node) override;
    void visit(const ParameterListNode* node) override;
    void visit(const VariableListNode* node) override;
    void visit(const TypedIdentifierNode* node) override;
    void visit(const TypeReferenceNode* node) override;
    void visit(const UnaryExpressionNode* node) override;
    void visit(const VariableNode* node) override;
    void visit(const VariableReferenceNode* node) override;
    void visit(const WhileStatementNode* node) override;
    void visit(const ProcedureDeclarationList* node) override;
};
