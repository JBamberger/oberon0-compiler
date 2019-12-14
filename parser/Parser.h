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
#include "ast/ProcedureBodyNode.h"
#include "ast/ProcedureCallNode.h"
#include "ast/ProcedureDeclarationNode.h"
#include "ast/ProcedureHeadingNode.h"
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

    const ModuleNode* module();
    const DeclarationsNode* declarations();
    const ConstantDeclarationList* const_declarations();
    const TypeDeclarationList* type_declarations();
    const VariableDeclarationList* var_declarations();
    const ProcedureDeclarationNode* procedure_declaration();
    const ProcedureHeadingNode* procedure_heading();
    const ProcedureBodyNode* procedure_body();
    const FormalParameterList* formal_parameters();
    const TypedIdentifierListNode<ParameterNode>* fp_section();
    const ExpressionNode* expression();
    const ExpressionNode* simple_expression();
    const ExpressionNode* term();
    const ExpressionNode* factor();
    const TypeNode* type();
    const ArrayTypeNode* array_type();
    const RecordTypeNode* record_type();
    const TypedIdentifierListNode<FieldNode>* field_list();
    const IdentifierListNode* ident_list();
    const StatementSequenceNode* statement_sequence();
    const StatementNode* statement();
    const AssignmentNode* assignment(const VariableReferenceNode* assignee);
    const ProcedureCallNode* procedure_call(const FilePos& pos, std::string name);
    const IfStatementNode* if_statement();
    const WhileStatementNode* while_statement();
    const ActualParameterNode* actual_parameters();
    const SelectorNode* selector();
    const StatementNode* procedure_call_or_assignment();

  public:
    explicit Parser(Scanner* scanner, Logger* logger);
    ~Parser();
    std::unique_ptr<const Node> parse();
};

#endif // OBERON0C_PARSER_H
