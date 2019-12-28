//
// Created by Michael Grossniklaus on 11/20/18.
//

#include "Parser.h"

#include "ElaborationErrors.h"
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

std::unique_ptr<const Token> Parser::require_token(const TokenType& type) const
{
    auto token = scanner_->nextToken();
    if (token->getType() != type) {
        const auto msg = "Expected " + type_str(type) + " but got " +
                         (std::stringstream() << *token).str() + ".";
        logger_->error(token->getPosition(), msg);
        exit(EXIT_FAILURE);
    }
    return token;
}

Identifier Parser::ident() const
{
    const auto token = require_token(TokenType::const_ident);
    const auto ident_token = dynamic_cast<const IdentToken*>(token.get());
    assert(ident_token != nullptr);

    return Identifier(ident_token->getValue(), ident_token->getPosition());
}

std::vector<Identifier> Parser::ident_list() const
{
    std::vector<Identifier> ids;
    const auto pos = scanner_->peekToken()->getPosition();
    ids.push_back(ident());

    while (scanner_->peekToken()->getType() == TokenType::comma) {
        static_cast<void>(scanner_->nextToken());
        ids.push_back(ident());
    }

    return ids;
}

std::unique_ptr<ModuleNode> Parser::module()
{
    const auto pos = require_token(TokenType::kw_module)->getPosition();
    const auto id1 = ident();

    auto module_node = std::make_unique<ModuleNode>(pos, id1.name);
    current_scope_ = module_node->getScope(); // enter scope

    // add the built-in types to the module scope
    addType(BasicTypeNode::makeInt());
    addType(BasicTypeNode::makeBool());
    addType(BasicTypeNode::makeString());

    static_cast<void>(require_token(TokenType::semicolon));
    declarations(module_node.get());
    if (scanner_->peekToken()->getType() == TokenType::kw_begin) {
        static_cast<void>(scanner_->nextToken());
        statement_sequence(module_node->getStatements().get());
    }
    static_cast<void>(require_token(TokenType::kw_end));
    const auto id2 = ident();
    static_cast<void>(require_token(TokenType::period));

    // check for E028
    if (id1.name != id2.name) {
        logger_->error(id2.name,
                       "Expected equal module names but got " + id1 + " and " + id2 + ".");
        exit(EXIT_FAILURE);
    }

    current_scope_ = nullptr; // exit scope

    return module_node;
}

void Parser::declarations(BlockNode* block)
{
    auto next = scanner_->peekToken();
    const auto pos = next->getPosition();

    if (next->getType() == TokenType::kw_const) {
        static_cast<void>(scanner_->nextToken()); // const keyword

        next = scanner_->peekToken();
        while (next->getType() == TokenType::const_ident) {
            const_declaration(block->getConstants().get());
            next = scanner_->peekToken();
        }
    }

    if (next->getType() == TokenType::kw_type) {
        static_cast<void>(scanner_->nextToken()); // type keyword

        next = scanner_->peekToken();
        while (next->getType() == TokenType::const_ident) {
            type_declaration(block->getTypes().get());
            next = scanner_->peekToken();
        }
    }

    if (next->getType() == TokenType::kw_var) {
        static_cast<void>(scanner_->nextToken()); // var keyword

        next = scanner_->peekToken();
        while (next->getType() == TokenType::const_ident) {
            var_declaration(block->getVariables().get());
            next = scanner_->peekToken();
        }
    }

    while (next->getType() == TokenType::kw_procedure) {
        procedure_declaration(block->getProcedures().get());
        static_cast<void>(require_token(TokenType::semicolon));
        next = scanner_->peekToken();
    }
}

void Parser::const_declaration(std::vector<std::unique_ptr<ConstantDeclarationNode>>* list)
{
    const auto id = ident();
    static_cast<void>(require_token(TokenType::op_eq));
    auto value = expression();
    static_cast<void>(require_token(TokenType::semicolon));

    auto node = std::make_unique<ConstantDeclarationNode>(id.pos, id.name, std::move(value));
    insertDeclaration(std::move(node), list);
}

void Parser::type_declaration(std::vector<std::unique_ptr<TypeDeclarationNode>>* list)
{
    const auto id = ident();
    static_cast<void>(require_token(TokenType::op_eq));
    auto tp = type();
    static_cast<void>(require_token(TokenType::semicolon));

    if (current_scope_->declareIdentifier(id.name, findType(tp, id.pos))) {
        list->push_back(std::make_unique<TypeDeclarationNode>(id.pos, id.name, std::move(tp)));
    } else {
        logger_->error(id.pos, errorDuplicateIdentifier(id.name));
        exit(EXIT_FAILURE);
    }
}

void Parser::var_declaration(std::vector<std::unique_ptr<VariableDeclarationNode>>* list)
{
    const auto ids = ident_list();
    static_cast<void>(require_token(TokenType::colon));
    auto tp = type();
    static_cast<void>(require_token(TokenType::semicolon));

    for (const auto& id : ids) {
        auto node = std::make_unique<VariableDeclarationNode>(id.pos, id.name, tp);
        insertDeclaration(std::move(node), list);
    }
}

void Parser::procedure_declaration(std::vector<std::unique_ptr<ProcedureDeclarationNode>>* list)
{
    // procedure heading
    const auto pos = require_token(TokenType::kw_procedure)->getPosition();
    const auto id1 = ident();

    auto node = std::make_unique<ProcedureDeclarationNode>(pos, id1.name, current_scope_);
    current_scope_ = node->getScope(); // enter block scope

    if (scanner_->peekToken()->getType() == TokenType::lparen) {
        formal_parameters(node->getParams().get());
    }
    static_cast<void>(require_token(TokenType::semicolon));

    // procedure body
    declarations(node.get());

    if (scanner_->peekToken()->getType() == TokenType::kw_begin) {
        static_cast<void>(scanner_->nextToken());
        statement_sequence(node->getStatements().get());
    }

    static_cast<void>(require_token(TokenType::kw_end));
    const auto id2 = ident();

    if (id1.name != id2.name) {
        logger_->error(pos, "Expected equal procedure names but got " + id1 + " and " + id2 + ".");
        exit(EXIT_FAILURE);
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

        logger_->error(id.pos, "Unknown identifier type.");
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
        static_cast<void>(require_token(TokenType::lparen));
        auto expr = expression();
        static_cast<void>(require_token(TokenType::rparen));
        return expr;
    }
    case TokenType::op_not: {
        auto op = require_token(TokenType::op_not);
        auto value = factor();
        return evaluateUnaryExpression(std::move(value), std::move(op));
    }
    default:
        logger_->error(next->getPosition(), "Expected Identifier, Number, lparen or ~ but got " +
                                                (std::stringstream() << *next).str() + ".");
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

        // the type must already exist because it is not allowed to define new basic types (E005,
        // E007, E008)
        const auto resolved_type = dynamic_cast<TypeNode*>(resolveId(current_scope_.get(), id));
        if (resolved_type == nullptr) {
            logger_->error(id.pos, "Identifier is not a type.");
            exit(EXIT_FAILURE);
        }

        // this must be getId because the identifier could be the name of a typedef
        return resolved_type->getId();
    }
    case TokenType::kw_array: {
        const auto pos = require_token(TokenType::kw_array)->getPosition();
        const auto array_value = expression();
        static_cast<void>(require_token(TokenType::kw_of));
        auto array_type = type();

        // check for E003
        const auto constant = dynamic_cast<NumberConstantNode*>(array_value.get());
        if (constant == nullptr) {
            logger_->error(array_value->getFilePos(), errorExpressionNotConst());
            exit(EXIT_FAILURE);
        }

        // check for E004
        const auto size = constant->getValue();
        if (0 >= size) {
            logger_->error(array_value->getFilePos(), errorSizeLtZero(size));
            exit(EXIT_FAILURE);
        }

        return addType(std::make_unique<ArrayTypeNode>(pos, size, std::move(array_type)));
    }
    case TokenType::kw_record: {
        const auto pos = scanner_->peekToken()->getPosition();

        auto type = std::make_unique<RecordTypeNode>(pos, current_scope_);
        current_scope_ = type->getScope(); // enter scope

        static_cast<void>(require_token(TokenType::kw_record));
        field_list(type->getMembers());
        while (scanner_->peekToken()->getType() == TokenType::semicolon) {
            static_cast<void>(require_token(TokenType::semicolon));
            field_list(type->getMembers());
        }
        static_cast<void>(require_token(TokenType::kw_end));

        current_scope_ = type->getScope()->getParent(); // exit scope
        return addType(std::move(type));
    }
    default: {
        const auto tt = (std::stringstream() << *next).str();
        logger_->error(next->getPosition(), "Expected name, ARRAY or RECORD but got " + tt + ".");
        exit(EXIT_FAILURE);
    }
    }
}

void Parser::field_list(std::vector<std::unique_ptr<FieldDeclarationNode>>* list)
{
    if (scanner_->peekToken()->getType() != TokenType::const_ident) {
        return;
    }
    const auto ids = ident_list();
    static_cast<void>(require_token(TokenType::colon));
    auto tp = type();

    for (const auto& id : ids) {
        auto node = std::make_unique<FieldDeclarationNode>(id.pos, id.name, tp);
        insertDeclaration(std::move(node), list);
    }
}

void Parser::formal_parameters(std::vector<std::unique_ptr<ParameterDeclarationNode>>* list)
{
    const auto pos = require_token(TokenType::lparen)->getPosition();

    if (scanner_->peekToken()->getType() == TokenType::kw_var ||
        scanner_->peekToken()->getType() == TokenType::const_ident) {

        fp_section(list);

        while (scanner_->peekToken()->getType() == TokenType::semicolon) {
            static_cast<void>(scanner_->nextToken());

            fp_section(list);
        }
    }

    static_cast<void>(require_token(TokenType::rparen));
}

void Parser::fp_section(std::vector<std::unique_ptr<ParameterDeclarationNode>>* list)
{
    const auto next_token = scanner_->peekToken();

    auto is_reference = false;
    if (next_token->getType() == TokenType::kw_var) {
        static_cast<void>(scanner_->nextToken());
        is_reference = true;
    }

    const auto ids = ident_list();
    static_cast<void>(require_token(TokenType::colon));
    auto tp = type();

    for (const auto& id : ids) {
        auto node = std::make_unique<ParameterDeclarationNode>(id.pos, id.name, tp, is_reference);
        insertDeclaration(std::move(node), list);
    }
}

void Parser::statement_sequence(std::vector<std::unique_ptr<StatementNode>>* list)
{
    list->emplace_back(statement());
    while (scanner_->peekToken()->getType() == TokenType::semicolon) {
        static_cast<void>(scanner_->nextToken());
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
        logger_->error(next->getPosition(), "Expected Identifier, IF or WHILE but got " +
                                                (std::stringstream() << *next).str() + ".");
        exit(EXIT_FAILURE);
    }
    }
}

std::unique_ptr<AssignmentNode> Parser::assignment(const Identifier& id)
{
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
            logger_->error(id.pos, errorLhsNotAssignable());
            exit(EXIT_FAILURE);
        }
    }

    auto lhs = selector(std::move(parent));

    static_cast<void>(require_token(TokenType::op_becomes));
    auto rhs = expression();

    return std::make_unique<AssignmentNode>(id.pos, std::move(lhs), std::move(rhs));
}

std::unique_ptr<ProcedureCallNode> Parser::procedure_call(const Identifier& id)
{
    auto actual = std::make_unique<std::vector<std::unique_ptr<ExpressionNode>>>();
    if (scanner_->peekToken()->getType() == TokenType::lparen) {
        actual_parameters(actual.get());
    }

    // check for E031: name must reference a procedure declaration
    auto proc_decl = dynamic_cast<ProcedureDeclarationNode*>(resolveId(current_scope_.get(), id));
    if (proc_decl == nullptr) {
        logger_->error(id.pos, "Identifier doesn't refer to a procedure.");
        exit(EXIT_FAILURE);
    }

    const auto formal = proc_decl->getParams().get();

    // check for E020: actual and formal param counts must match
    if (actual->size() != formal->size()) {
        logger_->error(
            id.pos,
            "Number of actual parameters does not match number of parameters of the declaration.");
        exit(EXIT_FAILURE);
    }

    // check for E021: actual and formal param types must match
    for (size_t i = 0; i < actual->size(); ++i) {
        const auto& a = actual->at(i);
        const auto& b = formal->at(i);
        if (a->getType() != b->getType()) {
            logger_->error(a->getFilePos(), "Parameter type mismatch. Expected " + b->getType() +
                                                " got " + a->getType() + ".");
            exit(EXIT_FAILURE);
        }
    }

    return std::make_unique<ProcedureCallNode>(id.pos, proc_decl, std::move(actual));
}

std::unique_ptr<IfStatementNode> Parser::if_statement()
{
    // if statement
    auto pos = require_token(TokenType::kw_if)->getPosition();
    auto cond = expression();
    static_cast<void>(require_token(TokenType::kw_then));
    auto node = std::make_unique<IfStatementNode>(pos, std::move(cond));

    statement_sequence(node->getThenPart().get());

    // this is the next node where the else body must be filled
    auto nextNode = node.get();

    // elif statements
    while (scanner_->peekToken()->getType() == TokenType::kw_elsif) {
        pos = scanner_->nextToken()->getPosition();
        cond = expression();
        auto tmp_node = std::make_unique<IfStatementNode>(pos, std::move(cond));
        const auto tmp_ptr = tmp_node.get();

        static_cast<void>(require_token(TokenType::kw_then));
        statement_sequence(tmp_node->getThenPart().get());

        nextNode->getElsePart()->push_back(std::move(tmp_node));
        nextNode = tmp_ptr;
    }

    // else statement
    if (scanner_->peekToken()->getType() == TokenType::kw_else) {
        static_cast<void>(scanner_->nextToken());
        statement_sequence(nextNode->getElsePart().get());
    }

    static_cast<void>(require_token(TokenType::kw_end));

    return node;
}

std::unique_ptr<WhileStatementNode> Parser::while_statement()
{
    const auto token = require_token(TokenType::kw_while);
    auto cond = expression();
    auto stmt = std::make_unique<WhileStatementNode>(token->getPosition(), std::move(cond));

    static_cast<void>(require_token(TokenType::kw_do));
    statement_sequence(stmt->getBody().get());
    static_cast<void>(require_token(TokenType::kw_end));

    return stmt;
}

void Parser::actual_parameters(std::vector<std::unique_ptr<ExpressionNode>>* params)
{
    static_cast<void>(require_token(TokenType::lparen));

    if (scanner_->peekToken()->getType() != TokenType::rparen) {
        params->push_back(expression());

        while (scanner_->peekToken()->getType() == TokenType::comma) {
            static_cast<void>(scanner_->nextToken());

            params->push_back(expression());
        }
    }

    static_cast<void>(require_token(TokenType::rparen));
}

std::unique_ptr<AssignableExpressionNode>
Parser::selector(std::unique_ptr<AssignableExpressionNode> parent)
{

    std::unique_ptr<AssignableExpressionNode> prev = std::move(parent);
    auto next = scanner_->peekToken();
    while (true) {
        if (next->getType() == TokenType::period) {
            static_cast<void>(scanner_->nextToken());
            const auto id = ident();

            // check for E018: field reference only on records
            const auto record_ref =
                dynamic_cast<RecordTypeNode*>(findType(prev->getType(), prev->getFilePos()));
            if (record_ref == nullptr) {
                logger_->error(prev->getFilePos(), "Ref is not a record type.");
                exit(EXIT_FAILURE);
            }

            // check for E019: field references can only use the local field declarations
            const auto field = dynamic_cast<FieldDeclarationNode*>(
                resolveLocalId(record_ref->getScope().get(), id));
            if (field == nullptr) {
                logger_->error(id.pos, "Field is not part of the record type.");
                exit(EXIT_FAILURE);
            }

            prev = std::make_unique<FieldReferenceNode>(id.pos, field, std::move(prev));

        } else if (next->getType() == TokenType::lbrack) {
            const auto open_token = scanner_->nextToken();
            auto index = expression();
            static_cast<void>(require_token(TokenType::rbrack));

            // check for E016: array indexing only on array types
            const auto arr =
                dynamic_cast<ArrayTypeNode*>(findType(prev->getType(), prev->getFilePos()));
            if (arr == nullptr) {
                logger_->error(open_token->getPosition(), "Array indexing on non-array type.");
                exit(EXIT_FAILURE);
            }

            // check for E030: array indices must be int
            const auto int_type =
                dynamic_cast<BasicTypeNode*>(findType(index->getType(), index->getFilePos()));
            if (int_type == nullptr || int_type->getName() != "INTEGER") {
                logger_->error(index->getFilePos(), "Index is not of type INTEGER.");
                exit(EXIT_FAILURE);
            }

            // check for E017: perform range check if the index is constant
            const auto const_idx = dynamic_cast<NumberConstantNode*>(index.get());
            if (const_idx != nullptr) {
                const auto value = const_idx->getValue();
                if (value < 0 || arr->getSize() <= value) {
                    logger_->error(index->getFilePos(), "Index out of range.");
                    exit(EXIT_FAILURE);
                }
            }

            prev = std::make_unique<ArrayReferenceNode>(open_token->getPosition(), std::move(index),
                                                        arr->getType(), std::move(prev));
        } else {
            break;
        }

        next = scanner_->peekToken();
    }
    return prev;
}

std::unique_ptr<ExpressionNode>
Parser::evaluateUnaryExpression(std::unique_ptr<ExpressionNode> operand,
                                const std::unique_ptr<const Token> op) const
{
    assert(operand != nullptr);
    const auto unary_op = toUnaryOperator(op->getType());

    // check for E012,E013,E014: types match operator
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

    // check for E012,E013,E014: types match operator
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

        if (operand_1->getType() != "BOOLEAN") {
            logger_->error(operand_1->getFilePos(), "Operand is not of type BOOLEAN.");
            exit(EXIT_FAILURE);
        }
        if (operand_2->getType() != "BOOLEAN") {
            logger_->error(operand_2->getFilePos(), "Operand is not of type BOOLEAN.");
            exit(EXIT_FAILURE);
        }
        return "BOOLEAN";
    }
    case OperatorType::arithmetic: {
        if (operand_1->getType() != "INTEGER") {
            logger_->error(operand_1->getFilePos(), "Operand is not of type INTEGER.");
            exit(EXIT_FAILURE);
        }
        if (operand_2->getType() != "INTEGER") {
            logger_->error(operand_2->getFilePos(), "Operand is not of type INTEGER.");
            exit(EXIT_FAILURE);
        }
        return "INTEGER";
    }
    case OperatorType::comparison: {
        if (operand_1->getType() != operand_2->getType()) {
            logger_->error(operand_1->getFilePos(), "Operands have different types.");
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

        if (operand->getType() != "BOOLEAN") {
            logger_->error(operand->getFilePos(), "Operand is not of type BOOLEAN.");
            exit(EXIT_FAILURE);
        }
        return "BOOLEAN";
    }
    case OperatorType::arithmetic: {
        if (operand->getType() != "INTEGER") {
            logger_->error(operand->getFilePos(), "Operand is not of type INTEGER.");
            exit(EXIT_FAILURE);
        }
        return "INTEGER";
    }
    case OperatorType::comparison:
        // there is no unary comparison operator
    default:
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
    if (resolved == nullptr) {
        logger_->error(pos, errorMissingDeclaration(name));
        exit(EXIT_FAILURE);
    }
    return resolved->value;
}

Node* Parser::resolveId(const Scope* scope, const std::string& name, const FilePos& pos) const
{
    const auto resolved = current_scope_->resolveIdentifier(name);
    if (resolved == nullptr) {
        logger_->error(pos, errorMissingDeclaration(name));
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
    if (record == types_.end()) {
        logger_->error(pos, errorMissingDeclaration(name));
        exit(EXIT_FAILURE);
    }
    return record->second.get();
}

Node* Parser::resolveId(const Scope* scope, const Identifier& id) const
{
    return resolveId(scope, id.name, id.pos);
}