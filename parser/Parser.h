//
// Created by Michael Grossniklaus on 2/2/18.
//

#ifndef OBERON0C_PARSER_H
#define OBERON0C_PARSER_H

#include "../scanner/Scanner.h"
#include "ast/ArrayTypeNode.h"
#include "ast/AssignmentNode.h"
#include "ast/ChainedReferenceNode.h"
#include "ast/ExpressionNode.h"
#include "ast/IfStatementNode.h"
#include "ast/Node.h"
#include "ast/ProcedureCallNode.h"
#include "ast/RecordTypeNode.h"
#include "ast/TypeNode.h"
#include "ast/WhileStatementNode.h"

class VariableReferenceNode;

class Parser {

  private:
    Scanner* scanner_;
    Logger* logger_;

    std::unique_ptr<const Token> require_token(const TokenType& type);
    std::string ident();

    const Node* module();
    const Node* declarations();
    const Node* const_declarations();
    const Node* type_declarations();
    const Node* var_declarations();
    const Node* procedure_declaration();
    const ExpressionNode* expression();
    const ExpressionNode* simple_expression();
    const ExpressionNode* term();
    const ExpressionNode* factor();
    const TypeNode* type();
    const ArrayTypeNode* array_type();
    const RecordTypeNode* record_type();
    const FieldListNode* field_list();
    const IdentifierListNode* ident_list();
    const Node* procedure_heading();
    const Node* procedure_body();
    const Node* formal_parameters();
    const Node* fp_section();
    const StatementSequenceNode* statement_sequence();
    const StatementNode* statement();
    const AssignmentNode* assignment(const VariableReferenceNode* assignee);
    const ProcedureCallNode* procedure_call(const VariableReferenceNode* name);
    const IfStatementNode* if_statement();
    const WhileStatementNode* while_statement();
    const ActualParameterNode* actual_parameters();
    const ChainedReferenceNode* selector();
    const StatementNode* procedure_call_or_assignment();

  public:
    explicit Parser(Scanner* scanner, Logger* logger);
    ~Parser();
    std::unique_ptr<const Node> parse();
};

#endif // OBERON0C_PARSER_H
