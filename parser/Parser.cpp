//
// Created by Michael Grossniklaus on 11/20/18.
//

#include "Parser.h"

#include "ParserErrors.h"
#include "StringToken.h"
#include "ast/ArrayReferenceNode.h"
#include "ast/BasicTypeNode.h"
#include "ast/BinaryExpressionNode.h"
#include "ast/BooleanConstantNode.h"
#include "ast/ConstantDeclarationNode.h"
#include "ast/FieldReferenceNode.h"
#include "ast/NumberConstantNode.h"
#include "ast/ParameterDeclarationNode.h"
#include "ast/ParameterReferenceNode.h"
#include "ast/StringConstantNode.h"
#include "ast/TypeDeclarationNode.h"
#include "ast/UnaryExpressionNode.h"
#include <IdentToken.h>
#include <NumberToken.h>
#include <cassert>
#include <iostream>
#include <sstream>

class ParameterReferenceNode;
Parser::Parser(Scanner* scanner, Logger* logger) : scanner_(scanner), logger_(logger) {}

Parser::~Parser() = default;

inline std::string type_str(const TokenType& t) { return (std::stringstream() << t).str(); }

std::unique_ptr<Node> Parser::parse()
{
    // new type map
    types_ = std::unordered_map<std::string, std::unique_ptr<TypeNode>>();
    return module();
}

bool Parser::checkToken(const TokenType& type) const
{
    return scanner_->peekToken()->getType() == type;
}

bool Parser::checkAndConsumeToken(const TokenType& type) const
{
    const auto consume = checkToken(type);
    if (consume) {
        static_cast<void>(scanner_->nextToken());
    }
    return consume;
}

void Parser::requireToken(const TokenType& type) const
{
    const auto token = scanner_->nextToken();

    // check for E000: syntax error
    if (token->getType() != type) {
        logError(token->getPosition(), error_id::E000, to_string(type), to_string(*token));
        exit(EXIT_FAILURE);
    }
}

std::unique_ptr<const Token> Parser::requireAndGetToken(const TokenType& type) const
{
    auto token = scanner_->nextToken();

    // check for E000: syntax error
    if (token->getType() != type) {
        logError(token->getPosition(), error_id::E000, to_string(type), to_string(*token));
        exit(EXIT_FAILURE);
    }
    return token;
}

Identifier Parser::ident() const
{
    const auto token = requireAndGetToken(TokenType::const_ident);
    const auto ident_token = dynamic_cast<const IdentToken*>(token.get());
    assert(ident_token != nullptr);

    return Identifier(ident_token->getValue(), ident_token->getPosition());
}

std::vector<Identifier> Parser::ident_list() const
{
    std::vector<Identifier> ids;
    const auto pos = scanner_->peekToken()->getPosition();
    ids.push_back(ident());

    while (checkAndConsumeToken(TokenType::comma)) {
        ids.push_back(ident());
    }

    return ids;
}

std::unique_ptr<ModuleNode> Parser::module()
{
    const auto pos = requireAndGetToken(TokenType::kw_module)->getPosition();
    const auto id1 = ident();

    auto module_node = std::make_unique<ModuleNode>(pos, id1.name);
    current_scope_ = module_node->getScope(); // enter scope

    // add the built-in types to the module scope
    static_cast<void>(addType(BasicTypeNode::makeInt()));
    static_cast<void>(addType(BasicTypeNode::makeBool()));
    static_cast<void>(addType(BasicTypeNode::makeString()));

    requireToken(TokenType::semicolon);
    declarations(module_node.get());
    if (checkAndConsumeToken(TokenType::kw_begin)) {
        statement_sequence(module_node->getStatements().get());
    }
    requireToken(TokenType::kw_end);
    const auto id2 = ident();
    requireToken(TokenType::period);

    // check for E028
    if (id1.name != id2.name) {
        logError(id2.pos, error_id::E028, id1.name, id2.name);
        exit(EXIT_FAILURE);
    }

    current_scope_ = nullptr; // exit scope

    return module_node;
}

void Parser::declarations(BlockNode* block)
{
    if (checkAndConsumeToken(TokenType::kw_const)) {
        while (checkToken(TokenType::const_ident)) {
            const_declaration(block->getConstants().get());
        }
    }

    if (checkAndConsumeToken(TokenType::kw_type)) {
        while (checkToken(TokenType::const_ident)) {
            type_declaration(block->getTypes().get());
        }
    }

    if (checkAndConsumeToken(TokenType::kw_var)) {
        while (checkToken(TokenType::const_ident)) {
            var_declaration(block->getVariables().get());
        }
    }

    while (checkToken(TokenType::kw_procedure)) {
        procedure_declaration(block->getProcedures().get());
        requireToken(TokenType::semicolon);
    }
}

void Parser::const_declaration(std::vector<std::unique_ptr<ConstantDeclarationNode>>* list)
{
    const auto id = ident();
    requireToken(TokenType::op_eq);
    auto value = expression();
    requireToken(TokenType::semicolon);

    // check for E002: constant declarations must evaluate to constants
    if (dynamic_cast<ConstantNode*>(value.get()) == nullptr) {
        logError(value->getFilePos(), error_id::E002);
        exit(EXIT_FAILURE);
    }

    auto node = std::make_unique<ConstantDeclarationNode>(id.pos, id.name, std::move(value));
    insertDeclaration(std::move(node), list);
}

void Parser::type_declaration(std::vector<std::unique_ptr<TypeDeclarationNode>>* list)
{
    const auto id = ident();
    requireToken(TokenType::op_eq);
    auto tp = type();
    requireToken(TokenType::semicolon);

    // check for E001: names must be unique
    if (!current_scope_->declareIdentifier(id.name, findType(tp, id.pos))) {
        logError(id.pos, error_id::E001, id.name);
        exit(EXIT_FAILURE);
    }

    list->push_back(std::make_unique<TypeDeclarationNode>(id.pos, id.name, std::move(tp)));
}

void Parser::var_declaration(std::vector<std::unique_ptr<VariableDeclarationNode>>* list)
{
    const auto ids = ident_list();
    requireToken(TokenType::colon);
    auto tp = type();
    requireToken(TokenType::semicolon);

    for (const auto& id : ids) {
        auto node = std::make_unique<VariableDeclarationNode>(id.pos, id.name, tp);
        insertDeclaration(std::move(node), list);
    }
}

void Parser::procedure_declaration(std::vector<std::unique_ptr<ProcedureDeclarationNode>>* list)
{
    // procedure heading
    const auto pos = requireAndGetToken(TokenType::kw_procedure)->getPosition();
    const auto id1 = ident();

    auto node = std::make_unique<ProcedureDeclarationNode>(pos, id1.name, current_scope_);
    current_scope_ = node->getScope(); // enter block scope

    if (checkToken(TokenType::lparen)) {
        formal_parameters(node->getParams().get());
    }
    requireToken(TokenType::semicolon);

    // procedure body
    declarations(node.get());

    if (checkAndConsumeToken(TokenType::kw_begin)) {
        statement_sequence(node->getStatements().get());
    }

    requireToken(TokenType::kw_end);
    const auto id2 = ident();

    // check for E029: procedure names must match
    if (id1.name != id2.name) {
        logError(pos, error_id::E029, id1.name, id2.name);
        exit(EXIT_FAILURE);
    }

    for (const auto& a : *node->getParams()) {
        const auto arg_type = findType(a->getType(), a->getFilePos());

        // check for E010: ARRAYs cannot be passed as VAR
        const auto array_type = dynamic_cast<ArrayTypeNode*>(arg_type);
        if (array_type != nullptr && a->isIsReference()) {
            logError(a->getFilePos(), error_id::E010);
            exit(EXIT_FAILURE);
        }

        // check for E011: RECORDs cannot be passed as VAR
        const auto record_type = dynamic_cast<RecordTypeNode*>(arg_type);
        if (record_type != nullptr && a->isIsReference()) {
            logError(a->getFilePos(), error_id::E011);
            exit(EXIT_FAILURE);
        }
    }

    current_scope_ = node->getScope()->getParent(); // exit block scope

    // TODO: is overloading allowed? Then the name must be augmented with the parameter types
    insertDeclaration(std::move(node), list);
}

std::unique_ptr<ExpressionNode> Parser::expression()
{
    auto operand_1 = simple_expression();
    const auto maybe_operand = scanner_->peekToken()->getType();
    if (maybe_operand == TokenType::op_eq || maybe_operand == TokenType::op_neq ||
        maybe_operand == TokenType::op_lt || maybe_operand == TokenType::op_leq ||
        maybe_operand == TokenType::op_gt || maybe_operand == TokenType::op_geq) {
        auto op = scanner_->nextToken();
        auto operand_2 = simple_expression();

        return evaluateBinaryExpression(std::move(operand_1), std::move(operand_2), std::move(op));
    }

    return operand_1;
}

std::unique_ptr<ExpressionNode> Parser::simple_expression()
{
    std::unique_ptr<ExpressionNode> operand_1;

    auto next = scanner_->peekToken()->getType();
    if (next == TokenType::op_plus || next == TokenType::op_minus) {
        auto op = scanner_->nextToken();
        auto operand = term();
        operand_1 = evaluateUnaryExpression(std::move(operand), std::move(op));
    } else {
        operand_1 = term();
    }

    next = scanner_->peekToken()->getType();
    while (next == TokenType::op_plus || next == TokenType::op_minus || next == TokenType::op_or) {
        auto op = scanner_->nextToken();
        auto operand_2 = term();
        operand_1 =
            evaluateBinaryExpression(std::move(operand_1), std::move(operand_2), std::move(op));
        next = scanner_->peekToken()->getType();
    }
    return operand_1;
}

std::unique_ptr<ExpressionNode> Parser::term()
{
    auto operand_1 = factor();
    auto next = scanner_->peekToken()->getType();
    while (next == TokenType::op_times || next == TokenType::op_div || next == TokenType::op_mod ||
           next == TokenType::op_and) {
        auto op = scanner_->nextToken();
        auto operand_2 = factor();
        operand_1 =
            evaluateBinaryExpression(std::move(operand_1), std::move(operand_2), std::move(op));
        next = scanner_->peekToken()->getType();
    }

    return operand_1;
}

std::unique_ptr<ExpressionNode> Parser::factor()
{
    const auto next = scanner_->peekToken();
    const auto pos = next->getPosition();

    switch (next->getType()) {
    case TokenType::const_ident: {
        const auto id = ident();
        const auto resolved = resolveId(current_scope_.get(), id);

        // check for E023: must be const, variable or parameter

        // check if the identifier is a constant
        const auto const_decl = dynamic_cast<ConstantDeclarationNode*>(resolved);
        if (const_decl != nullptr) {
            const auto num_const = dynamic_cast<NumberConstantNode*>(const_decl->getValue().get());
            if (num_const != nullptr) {
                return std::make_unique<NumberConstantNode>(id.pos, num_const->getValue());
            }

            const auto str_const = dynamic_cast<StringConstantNode*>(const_decl->getValue().get());
            if (str_const != nullptr) {
                return std::make_unique<StringConstantNode>(id.pos, str_const->getValue());
            }
        }

        // check if the identifier is a variable
        const auto var_decl = dynamic_cast<VariableDeclarationNode*>(resolved);
        if (var_decl != nullptr) {
            return selector(std::make_unique<VariableReferenceNode>(id.pos, var_decl));
        }

        // check if the identifier is a parameter
        const auto param_decl = dynamic_cast<ParameterDeclarationNode*>(resolved);
        if (param_decl != nullptr) {
            return selector(std::make_unique<ParameterReferenceNode>(id.pos, param_decl));
        }

        logError(id.pos, error_id::E023, id.name);
        exit(EXIT_FAILURE);
    }
    case TokenType::const_number: {
        const auto token = scanner_->nextToken();
        const auto num_token = dynamic_cast<const NumberToken*>(token.get());
        assert(num_token != nullptr);
        const auto value = num_token->getValue();

        return std::make_unique<NumberConstantNode>(pos, value);
    }
    case TokenType::const_string: {
        const auto token = scanner_->nextToken();
        const auto str_token = dynamic_cast<const StringToken*>(token.get());
        assert(str_token != nullptr);
        const auto value = str_token->getValue();

        return std::make_unique<StringConstantNode>(pos, value);
    }
    case TokenType::lparen: {
        requireToken(TokenType::lparen);
        auto expr = expression();
        requireToken(TokenType::rparen);
        return expr;
    }
    case TokenType::op_not: {
        auto op = requireAndGetToken(TokenType::op_not);
        auto value = factor();
        return evaluateUnaryExpression(std::move(value), std::move(op));
    }
    default:
        logError(next->getPosition(), error_id::E000, "identifier, number, lparen or ~",
                 to_string(*next));
        exit(EXIT_FAILURE);
    }
}

std::string Parser::type()
{
    // unique names from record/arrays:
    // ARRAY 10 OF INTEGER --> [A,10,INTEGER;]
    // RECORD a,b: INTEGER; c: RECORD x: BOOLEAN END; END -->
    // [R;a,INTEGER;b,INTEGER;c,[R;x,BOOLEAN;]]

    const auto next = scanner_->peekToken();

    switch (next->getType()) {
    case TokenType::const_ident: {
        const auto id = ident();

        // check for E005: the type must already exist because it is not allowed to define
        // new basic types
        const auto resolved_type = dynamic_cast<TypeNode*>(resolveId(current_scope_.get(), id));
        if (resolved_type == nullptr) {
            logError(id.pos, error_id::E005, id.name);
            exit(EXIT_FAILURE);
        }

        // this must be getId because the identifier could be the name of a typedef
        return resolved_type->getId();
    }
    case TokenType::kw_array: {
        const auto pos = requireAndGetToken(TokenType::kw_array)->getPosition();
        const auto array_value = expression();
        requireToken(TokenType::kw_of);
        auto array_type = type();

        // check for E003: array size must be constant
        const auto constant = dynamic_cast<NumberConstantNode*>(array_value.get());
        if (constant == nullptr) {
            logError(array_value->getFilePos(), error_id::E003);
            exit(EXIT_FAILURE);
        }

        // check for E004: array size must be gt 0
        const auto size = constant->getValue();
        if (0 >= size) {
            logError(array_value->getFilePos(), error_id::E004, size);
            exit(EXIT_FAILURE);
        }

        return addType(std::make_unique<ArrayTypeNode>(pos, size, std::move(array_type)));
    }
    case TokenType::kw_record: {
        const auto pos = scanner_->peekToken()->getPosition();

        auto type = std::make_unique<RecordTypeNode>(pos, current_scope_);
        current_scope_ = type->getScope(); // enter scope

        requireToken(TokenType::kw_record);
        field_list(type->getMembers());
        while (checkAndConsumeToken(TokenType::semicolon)) {
            field_list(type->getMembers());
        }
        requireToken(TokenType::kw_end);

        current_scope_ = type->getScope()->getParent(); // exit scope
        return addType(std::move(type));
    }
    default: {
        logError(next->getPosition(), error_id::E000, "name, ARRAY or RECORD", to_string(*next));
        exit(EXIT_FAILURE);
    }
    }
}

void Parser::field_list(std::vector<std::unique_ptr<FieldDeclarationNode>>* list)
{
    if (!checkToken(TokenType::const_ident)) {
        return;
    }
    const auto ids = ident_list();
    requireToken(TokenType::colon);
    auto tp = type();

    for (const auto& id : ids) {
        auto node = std::make_unique<FieldDeclarationNode>(id.pos, id.name, tp);
        insertDeclaration(std::move(node), list);
    }
}

void Parser::formal_parameters(std::vector<std::unique_ptr<ParameterDeclarationNode>>* list)
{
    requireToken(TokenType::lparen);

    if (checkToken(TokenType::kw_var) || checkToken(TokenType::const_ident)) {
        fp_section(list);
        while (checkAndConsumeToken(TokenType::semicolon)) {
            fp_section(list);
        }
    }
    requireToken(TokenType::rparen);
}

void Parser::fp_section(std::vector<std::unique_ptr<ParameterDeclarationNode>>* list)
{
    const auto is_reference = checkAndConsumeToken(TokenType::kw_var);
    const auto ids = ident_list();
    requireToken(TokenType::colon);
    auto tp = type();

    for (const auto& id : ids) {
        auto node = std::make_unique<ParameterDeclarationNode>(id.pos, id.name, tp, is_reference);
        insertDeclaration(std::move(node), list);
    }
}

void Parser::statement_sequence(std::vector<std::unique_ptr<StatementNode>>* list)
{
    list->emplace_back(statement());
    while (checkAndConsumeToken(TokenType::semicolon)) {
        list->emplace_back(statement());
    }
}

std::unique_ptr<StatementNode> Parser::statement()
{
    switch (scanner_->peekToken()->getType()) {
    case TokenType::const_ident: {
        const auto id = ident();

        switch (scanner_->peekToken()->getType()) {
        case TokenType::period:
        case TokenType::lbrack:
        case TokenType::op_becomes:
            return assignment(id);
        default:
            return procedure_call(id);
        }
    }
    case TokenType::kw_if:
        return if_statement();
    case TokenType::kw_while:
        return while_statement();
    default: {
        const auto next = scanner_->peekToken();
        logError(next->getPosition(), error_id::E000, "name, IF or WHILE", to_string(*next));
        exit(EXIT_FAILURE);
    }
    }
}

std::unique_ptr<AssignmentNode> Parser::assignment(const Identifier& id)
{
    // check for E024: lhs must be assignable
    const auto symbol = resolveId(current_scope_.get(), id);
    std::unique_ptr<AssignableExpressionNode> parent = nullptr;
    const auto var_decl = dynamic_cast<VariableDeclarationNode*>(symbol);
    if (var_decl != nullptr) {
        parent = std::make_unique<VariableReferenceNode>(id.pos, var_decl);
    } else {
        const auto param_decl = dynamic_cast<ParameterDeclarationNode*>(symbol);
        if (param_decl != nullptr) {
            parent = std::make_unique<ParameterReferenceNode>(id.pos, param_decl);
        } else {
            logError(id.pos, error_id::E024);
            exit(EXIT_FAILURE);
        }
    }

    auto lhs = selector(std::move(parent));

    requireToken(TokenType::op_becomes);
    auto rhs = expression();

    // check for E025: assignment types must match
    if (lhs->getType() != rhs->getType()) {
        logError(rhs->getFilePos(), error_id::E025, lhs->getType(), rhs->getType());
        exit(EXIT_FAILURE);
    }

    return std::make_unique<AssignmentNode>(id.pos, std::move(lhs), std::move(rhs));
}

std::unique_ptr<ProcedureCallNode> Parser::procedure_call(const Identifier& id)
{
    auto actual = std::make_unique<std::vector<std::unique_ptr<ExpressionNode>>>();
    if (checkToken(TokenType::lparen)) {
        actual_parameters(actual.get());
    }

    // check for E031: name must reference a procedure declaration
    auto proc_decl = dynamic_cast<ProcedureDeclarationNode*>(resolveId(current_scope_.get(), id));
    if (proc_decl == nullptr) {
        logError(id.pos, error_id::E031, id.name);
        exit(EXIT_FAILURE);
    }

    const auto formal = proc_decl->getParams().get();

    // check for E020: actual and formal param counts must match
    if (actual->size() != formal->size()) {
        logError(id.pos, error_id::E020, actual->size(), formal->size(), id.name);
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < actual->size(); ++i) {
        const auto& a = actual->at(i);
        const auto& b = formal->at(i);

        // check for E021: actual and formal param types must match
        if (a->getType() != b->getType()) {
            logError(a->getFilePos(), error_id::E021, b->getType(), a->getType());
            exit(EXIT_FAILURE);
        }

        if (b->isIsReference()) {
            // check for E022: VAR parameter must be addressable and assignable
            const auto assignable = dynamic_cast<AssignableExpressionNode*>(a.get());
            if (assignable == nullptr) {
                logError(a->getFilePos(), error_id::E022, b->getName());
                exit(EXIT_FAILURE);
            }
        }
    }

    return std::make_unique<ProcedureCallNode>(id.pos, proc_decl, std::move(actual));
}

std::unique_ptr<IfStatementNode> Parser::if_statement()
{
    // if condition
    auto pos = requireAndGetToken(TokenType::kw_if)->getPosition();
    auto cond = expression();
    requireToken(TokenType::kw_then);

    // check for E026: if condition must be BOOLEAN
    if (cond->getType() != "BOOLEAN") {
        logError(cond->getFilePos(), error_id::E026, cond->getType());
        exit(EXIT_FAILURE);
    }

    auto node = std::make_unique<IfStatementNode>(pos, std::move(cond));
    // if body
    statement_sequence(node->getThenPart().get());

    // this is the next node where the else body must be filled
    auto next_node = node.get();

    // elif statements
    while (checkToken(TokenType::kw_elsif)) {
        // elif condition
        pos = requireAndGetToken(TokenType::kw_elsif)->getPosition();
        cond = expression();
        requireToken(TokenType::kw_then);

        // check for E026: if condition must be BOOLEAN
        if (cond->getType() != "BOOLEAN") {
            logError(cond->getFilePos(), error_id::E026, cond->getType());
            exit(EXIT_FAILURE);
        }

        auto tmp_node = std::make_unique<IfStatementNode>(pos, std::move(cond));
        const auto tmp_ptr = tmp_node.get();

        // elif body
        statement_sequence(tmp_node->getThenPart().get());

        next_node->getElsePart()->push_back(std::move(tmp_node));
        next_node = tmp_ptr;
    }

    // else statement
    if (checkAndConsumeToken(TokenType::kw_else)) {
        statement_sequence(next_node->getElsePart().get());
    }
    requireToken(TokenType::kw_end);

    return node;
}

std::unique_ptr<WhileStatementNode> Parser::while_statement()
{
    const auto pos = requireAndGetToken(TokenType::kw_while)->getPosition();
    auto cond = expression();

    // check for E027: while condition must be BOOLEAN
    if (cond->getType() != "BOOLEAN") {
        logError(cond->getFilePos(), error_id::E027, cond->getType());
        exit(EXIT_FAILURE);
    }

    auto stmt = std::make_unique<WhileStatementNode>(pos, std::move(cond));

    requireToken(TokenType::kw_do);
    statement_sequence(stmt->getBody().get());
    requireToken(TokenType::kw_end);

    return stmt;
}

void Parser::actual_parameters(std::vector<std::unique_ptr<ExpressionNode>>* params)
{
    requireToken(TokenType::lparen);

    if (!checkToken(TokenType::rparen)) {
        params->push_back(expression());
        while (checkAndConsumeToken(TokenType::comma)) {
            params->push_back(expression());
        }
    }
    requireToken(TokenType::rparen);
}

std::unique_ptr<AssignableExpressionNode>
Parser::selector(std::unique_ptr<AssignableExpressionNode> parent)
{
    auto prev = std::move(parent);
    while (true) {
        if (checkAndConsumeToken(TokenType::period)) {
            const auto id = ident();

            // check for E018: field reference only on records
            const auto record_ref =
                dynamic_cast<RecordTypeNode*>(findType(prev->getType(), prev->getFilePos()));
            if (record_ref == nullptr) {
                logError(id.pos, error_id::E018, prev->getType());
                exit(EXIT_FAILURE);
            }

            // check for E019: field references can only use the local field declarations
            const auto field = dynamic_cast<FieldDeclarationNode*>(
                resolveLocalId(record_ref->getScope().get(), id));
            if (field == nullptr) {
                logError(id.pos, error_id::E019, id.name, prev->getType());
                exit(EXIT_FAILURE);
            }

            prev = std::make_unique<FieldReferenceNode>(id.pos, field, std::move(prev));

        } else if (checkToken(TokenType::lbrack)) {
            const auto open_token = requireAndGetToken(TokenType::lbrack);
            auto index = expression();
            requireToken(TokenType::rbrack);

            // check for E016: array indexing only on array types
            const auto arr =
                dynamic_cast<ArrayTypeNode*>(findType(prev->getType(), prev->getFilePos()));
            if (arr == nullptr) {
                logError(open_token->getPosition(), error_id::E016, prev->getType());
                exit(EXIT_FAILURE);
            }

            // check for E030: array indices must be int
            const auto int_type =
                dynamic_cast<BasicTypeNode*>(findType(index->getType(), index->getFilePos()));
            if (int_type == nullptr || int_type->getName() != "INTEGER") {
                logError(index->getFilePos(), error_id::E030, index->getType());
                exit(EXIT_FAILURE);
            }

            // check for E017: perform range check if the index is constant
            const auto const_idx = dynamic_cast<NumberConstantNode*>(index.get());
            if (const_idx != nullptr) {
                const auto value = const_idx->getValue();
                if (value < 0 || arr->getSize() <= value) {
                    logError(index->getFilePos(), error_id::E017, value, arr->getSize());
                    exit(EXIT_FAILURE);
                }
            }

            prev = std::make_unique<ArrayReferenceNode>(open_token->getPosition(), std::move(index),
                                                        arr->getType(), std::move(prev));
        } else {
            break;
        }
    }
    return prev;
}

std::unique_ptr<ExpressionNode>
Parser::evaluateUnaryExpression(std::unique_ptr<ExpressionNode> operand,
                                const std::unique_ptr<const Token> op) const
{
    assert(operand != nullptr);
    const auto unary_op = toUnaryOperator(op->getType());

    const auto result_type = typeCheckUnary(operand.get(), getOperatorType(unary_op));

    // T001: evaluate constants
    const auto first = dynamic_cast<NumberConstantNode*>(operand.get());
    if (first != nullptr) {
        const auto value = evalUnary(unary_op, first->getValue());

        if (result_type == "INTEGER") {
            return std::make_unique<NumberConstantNode>(first->getFilePos(), value);
        }
        return std::make_unique<BooleanConstantNode>(first->getFilePos(), value);
    }

    return std::make_unique<UnaryExpressionNode>(op->getPosition(), unary_op, std::move(operand));
}

std::unique_ptr<ExpressionNode>
Parser::evaluateBinaryExpression(std::unique_ptr<ExpressionNode> operand_1,
                                 std::unique_ptr<ExpressionNode> operand_2,
                                 const std::unique_ptr<const Token> op) const
{
    assert(operand_1 != nullptr);
    assert(operand_2 != nullptr);

    const auto bin_op = toBinaryOperator(op->getType());

    const auto result_type =
        typeCheckBinary(operand_1.get(), operand_2.get(), getOperatorType(bin_op));

    // T001: evaluate constants
    const auto first = dynamic_cast<NumberConstantNode*>(operand_1.get());
    const auto second = dynamic_cast<NumberConstantNode*>(operand_2.get());
    if (first != nullptr && second != nullptr) {
        const auto value = evalBinary(bin_op, first->getValue(), second->getValue());
        if (result_type == "INTEGER") {
            return std::make_unique<NumberConstantNode>(first->getFilePos(), value);
        }
        return std::make_unique<BooleanConstantNode>(first->getFilePos(), value);
    }

    return std::make_unique<BinaryExpressionNode>(operand_1->getFilePos(), bin_op,
                                                  std::move(operand_1), std::move(operand_2));
}

std::string
Parser::typeCheckBinary(ExpressionNode* operand_1, ExpressionNode* operand_2, OperatorType op) const
{
    switch (op) {
    case OperatorType::logical: {
        // check for E014: boolean ops only on BOOLEAN
        if (operand_1->getType() != "BOOLEAN") {
            logError(operand_1->getFilePos(), error_id::E014, operand_1->getType());
            exit(EXIT_FAILURE);
        }
        if (operand_2->getType() != "BOOLEAN") {
            logError(operand_2->getFilePos(), error_id::E014, operand_2->getType());
            exit(EXIT_FAILURE);
        }
        return "BOOLEAN";
    }
    case OperatorType::arithmetic: {
        // check for E012: arithmetic ops only on INTEGER
        if (operand_1->getType() != "INTEGER") {
            logError(operand_1->getFilePos(), error_id::E012, operand_1->getType());
            exit(EXIT_FAILURE);
        }
        if (operand_2->getType() != "INTEGER") {
            logError(operand_2->getFilePos(), error_id::E012, operand_2->getType());
            exit(EXIT_FAILURE);
        }
        return "INTEGER";
    }
    case OperatorType::comparison: {
        // check for E013: comparison types must be equal
        if (operand_1->getType() != operand_2->getType()) {
            logError(operand_1->getFilePos(), error_id::E013, operand_1->getType(),
                     operand_2->getType());
            exit(EXIT_FAILURE);
        }
        return "BOOLEAN";
    }
    default:
        std::terminate();
    }
}

std::string Parser::typeCheckUnary(ExpressionNode* operand, OperatorType op) const
{
    switch (op) {
    case OperatorType::logical: {
        // check for E014: boolean ops only on BOOLEAN
        if (operand->getType() != "BOOLEAN") {
            logError(operand->getFilePos(), error_id::E014, operand->getType());
            exit(EXIT_FAILURE);
        }
        return "BOOLEAN";
    }
    case OperatorType::arithmetic: {
        // check for E012: arithmetic ops only on INTEGER
        if (operand->getType() != "INTEGER") {
            logError(operand->getFilePos(), error_id::E012, operand->getType());
            exit(EXIT_FAILURE);
        }
        return "INTEGER";
    }
    default:
        // there is no unary comparison operator
        std::terminate();
    }
}

Node* Parser::resolveLocalId(const Scope* scope, const Identifier& id) const
{
    return resolveLocalId(scope, id.name, id.pos);
}

Node* Parser::resolveLocalId(const Scope* scope, const std::string& name, const FilePos& pos) const
{
    const auto resolved = current_scope_->resolveIdentifierLocally(name);

    // check for E015: identifiers must exist
    if (resolved == nullptr) {
        logError(pos, error_id::E015, name);
        exit(EXIT_FAILURE);
    }

    return resolved->value;
}

Node* Parser::resolveId(const Scope* scope, const std::string& name, const FilePos& pos) const
{
    const auto resolved = current_scope_->resolveIdentifier(name);

    // check for E015: identifiers must exist
    if (resolved == nullptr) {
        logError(pos, error_id::E015, name);
        exit(EXIT_FAILURE);
    }

    return resolved->value;
}

std::string Parser::addType(std::unique_ptr<TypeNode> type)
{
    const auto name = type->getId();
    // it doesn't matter if the type cannot be inserted as the types are the same if the descriptor
    // is the same
    static_cast<void>(current_scope_->declareIdentifier(name, type.get()));
    static_cast<void>(types_.insert({name, std::move(type)}));
    return name;
}

TypeNode* Parser::findType(const std::string& name, const FilePos& pos) const
{
    const auto record = types_.find(name);

    // check for E005: types must exist
    if (record == types_.end()) {
        logError(pos, error_id::E005, name);
        exit(EXIT_FAILURE);
    }

    return record->second.get();
}

Node* Parser::resolveId(const Scope* scope, const Identifier& id) const
{
    return resolveId(scope, id.name, id.pos);
}