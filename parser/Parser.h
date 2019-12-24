#pragma once

#include "../scanner/Scanner.h"
#include "ast/ArrayTypeNode.h"
#include "ast/AssignmentNode.h"
#include "ast/ExpressionNode.h"
#include "ast/Identifier.h"
#include "ast/IfStatementNode.h"
#include "ast/ModuleNode.h"
#include "ast/Node.h"
#include "ast/ProcedureCallNode.h"
#include "ast/ProcedureDeclarationNode.h"
#include "ast/RecordTypeNode.h"
#include "ast/SelectorNode.h"
#include "ast/TypeNode.h"
#include "ast/VariableDeclarationNode.h"
#include "ast/WhileStatementNode.h"

class Parser {

    Scanner* scanner_;
    Logger* logger_;
    std::shared_ptr<Scope> current_scope_;

  public:
    explicit Parser(Scanner* scanner, Logger* logger);
    ~Parser();
    std::unique_ptr<Node> parse();

  private:
    std::unique_ptr<const Token> require_token(const TokenType& type) const;
    Identifier ident() const;
    std::vector<Identifier> ident_list() const;

    std::unique_ptr<ModuleNode> module();
    void declarations(BlockNode* block);
    std::unique_ptr<ConstantDeclarationNode> const_declaration();
    std::unique_ptr<TypeDeclarationNode> type_declaration();
    void var_declaration(std::vector<std::unique_ptr<VariableDeclarationNode>>* var_list);
    std::unique_ptr<ProcedureDeclarationNode> procedure_declaration();
    void formal_parameters(ProcedureDeclarationNode* proc_decl);
    void fp_section(ProcedureDeclarationNode* proc_decl);
    std::unique_ptr<ExpressionNode> expression();
    std::unique_ptr<ExpressionNode> simple_expression();
    std::unique_ptr<ExpressionNode> term();
    std::unique_ptr<ExpressionNode> factor();
    std::unique_ptr<TypeNode> type();
    std::unique_ptr<ArrayTypeNode> array_type();
    std::unique_ptr<RecordTypeNode> record_type();
    void field_list(RecordTypeNode* rec_decl);
    void actual_parameters(std::vector<std::unique_ptr<ExpressionNode>>* params);
    void statement_sequence(std::vector<std::unique_ptr<StatementNode>>* list);
    std::unique_ptr<StatementNode> statement();
    std::unique_ptr<AssignmentNode> assignment(const Identifier& id);
    std::unique_ptr<ProcedureCallNode> procedure_call(const Identifier& id);
    std::unique_ptr<IfStatementNode> if_statement();
    std::unique_ptr<WhileStatementNode> while_statement();
    std::unique_ptr<SelectorNode> selector();




    static std::unique_ptr<ExpressionNode>
    evaluateBinaryExpression(std::unique_ptr<ExpressionNode> operand_1,
                             std::unique_ptr<ExpressionNode> operand_2,
                             std::unique_ptr<const Token> op);
    static std::unique_ptr<ExpressionNode>
    evaluateUnaryExpression(std::unique_ptr<ExpressionNode> operand,
                            std::unique_ptr<const Token> op);
};
