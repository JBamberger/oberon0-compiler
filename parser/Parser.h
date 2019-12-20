//
// Created by Michael Grossniklaus on 2/2/18.
//

#ifndef OBERON0C_PARSER_H
#define OBERON0C_PARSER_H

#include "../scanner/Scanner.h"
#include "ast/ArrayTypeNode.h"
#include "ast/AssignmentNode.h"
#include "ast/ExpressionNode.h"
#include "ast/IfStatementNode.h"
#include "ast/ModuleNode.h"
#include "ast/Node.h"
#include "ast/ProcedureCallNode.h"
#include "ast/ProcedureDeclarationNode.h"
#include "ast/RecordTypeNode.h"
#include "ast/SelectorNode.h"
#include "ast/TypeNode.h"
#include "ast/WhileStatementNode.h"

enum class UnaryOperator : char;
enum class BinaryOperator : char;
class VariableReferenceNode;

class Parser {

    // private:
    Scanner* scanner_;
    Logger* logger_;
    std::shared_ptr<Scope> current_scope_;

    std::unique_ptr<const Token> require_token(const TokenType& type) const;
    std::string ident() const;

    std::unique_ptr<ModuleNode> module();
    void declarations(BlockNode* block);
    std::unique_ptr<ConstantDeclarationNode> const_declaration();
    std::unique_ptr<TypeDeclarationNode> type_declaration();
    std::unique_ptr<VariableListNode> var_declaration();
    std::unique_ptr<ProcedureDeclarationNode> procedure_declaration();
    void formal_parameters(ProcedureDeclarationNode* proc_decl);
    ParameterListNode* fp_section();
    ExpressionNode* expression();
    ExpressionNode* simple_expression();
    ExpressionNode* term();
    ExpressionNode* factor();
    TypeNode* type();
    ArrayTypeNode* array_type();
    RecordTypeNode* record_type();
    FieldListNode* field_list();
    IdentifierListNode* ident_list();
    void statement_sequence(std::vector<std::unique_ptr<StatementNode>>* list);
    StatementNode* statement();
    AssignmentNode* assignment(const VariableReferenceNode* assignee);
    ProcedureCallNode* procedure_call(const FilePos& pos, std::string name);
    IfStatementNode* if_statement();
    WhileStatementNode* while_statement();
    ActualParameterNode* actual_parameters();
    SelectorNode* selector();
    StatementNode* procedure_call_or_assignment();

    static ExpressionNode* evaluateBinaryExpression(ExpressionNode* operand_1,
                                                    ExpressionNode* operand_2,
                                                    std::unique_ptr<const Token> op);
    static ExpressionNode* evaluateUnaryExpression(ExpressionNode* operand,
                                                   std::unique_ptr<const Token> op);

  public:
    explicit Parser(Scanner* scanner, Logger* logger);
    ~Parser();
    std::unique_ptr<Node> parse();
};

#endif // OBERON0C_PARSER_H
