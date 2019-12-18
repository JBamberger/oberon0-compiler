//
// Created by Michael Grossniklaus on 2/2/18.
//

#ifndef OBERON0C_PARSER_H
#define OBERON0C_PARSER_H

#include "../scanner/Scanner.h"
#include "ast/ArrayTypeNode.h"
#include "ast/AssignmentNode.h"
#include "ast/DeclarationsNode.h"
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

class VariableReferenceNode;

class Parser {

    // private:
    Scanner* scanner_;
    Logger* logger_;

    std::unique_ptr<const Token> require_token(const TokenType& type) const;
    std::string ident() const;

    ModuleNode* module();
    DeclarationsNode* declarations();
    ConstantDeclarationList* const_declarations();
    TypeDeclarationList* type_declarations();
    VariableDeclarationList* var_declarations();
    ProcedureDeclarationNode* procedure_declaration();
    FormalParameterList* formal_parameters();
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
    StatementSequenceNode* statement_sequence();
    StatementNode* statement();
    AssignmentNode* assignment(const VariableReferenceNode* assignee);
    ProcedureCallNode* procedure_call(const FilePos& pos, std::string name);
    IfStatementNode* if_statement();
    WhileStatementNode* while_statement();
    ActualParameterNode* actual_parameters();
    SelectorNode* selector();
    StatementNode* procedure_call_or_assignment();

  public:
    explicit Parser(Scanner* scanner, Logger* logger);
    ~Parser();
    std::unique_ptr<Node> parse();
};

#endif // OBERON0C_PARSER_H
