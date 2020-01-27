
#pragma once

#include "CodeGenerator.h"
#include "NodeVisitor.h"

/**
 * Simple stack-based x86_64 code generator.
 */
class X86_64CodeGenerator : public CodeGenerator, private NodeVisitor {
    const size_t LABEL_COL = 0;
    const size_t INSTR_COL = 8;
    const size_t PARAM_COL = 16;
    const size_t COMMENT_COL = 40;

    std::ostream* output_;
    std::string nl_;
    size_t label_nr_;
    std::shared_ptr<Scope> root_scope;
    std::shared_ptr<Scope> current_scope;

  public:
    X86_64CodeGenerator();

    ~X86_64CodeGenerator() override;

    void generate(std::unique_ptr<ModuleNode> ast, std::ostream* output) override;

  private:
    std::string nextLabel();

    std::string padRight(const std::string& value, size_t column);

    void defineConstants(const BlockNode* node);
    void defineVariables(const BlockNode* node);

    void visit(const ArrayReferenceNode* node) override;

    void visit(const ArrayTypeNode* node) override;

    void visit(const AssignmentNode* node) override;

    void visit(const BinaryExpressionNode* node) override;

    void visit(const UnaryExpressionNode* node) override;

    void visit(const ConstantDeclarationNode* node) override;

    void visit(const TypeDeclarationNode* node) override;

    void visit(const VariableDeclarationNode* node) override;

    void visit(const ProcedureDeclarationNode* node) override;

    void visit(const FieldReferenceNode* node) override;

    void visit(const FieldDeclarationNode* node) override;

    void visit(const IfStatementNode* node) override;

    void visit(const ModuleNode* node) override;

    void visit(const NumberConstantNode* node) override;

    void visit(const StringConstantNode* node) override;

    void visit(const ParameterDeclarationNode* node) override;

    void visit(const ProcedureCallNode* node) override;

    void visit(const RecordTypeNode* node) override;

    void visit(const TypedIdentifierNode* node) override;

    void visit(const BasicTypeNode* node) override;

    void visit(const VariableReferenceNode* node) override;

    void visit(const WhileStatementNode* node) override;
};
