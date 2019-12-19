//
// Created by Michael Grossniklaus on 11/20/18.
//

#include "Parser.h"

#include "StringToken.h"
#include "ast/ArrayReferenceNode.h"
#include "ast/BasicTypeNode.h"
#include "ast/BinaryExpressionNode.h"
#include "ast/ConstantDeclarationNode.h"
#include "ast/FieldReferenceNode.h"
#include "ast/NumberConstantNode.h"
#include "ast/SelectorNode.h"
#include "ast/StringConstantNode.h"
#include "ast/TypeDeclarationNode.h"
#include "ast/TypedIdentifierListNode.h"
#include "ast/UnaryExpressionNode.h"
#include <IdentToken.h>
#include <NumberToken.h>
#include <cassert>
#include <iostream>
#include <sstream>

Parser::Parser(Scanner* scanner, Logger* logger) : scanner_(scanner), logger_(logger) {}

Parser::~Parser() = default;

inline std::string type_str(const TokenType& t) { return (std::stringstream() << t).str(); }

std::unique_ptr<Node> Parser::parse() { return std::unique_ptr<Node>(module()); }

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

std::string Parser::ident() const
{
    const auto token = require_token(TokenType::const_ident);
    const auto ident_token = dynamic_cast<const IdentToken*>(token.get());
    assert(ident_token != nullptr);

    return ident_token->getValue();
}

ModuleNode* Parser::module()
{
    const auto pos = require_token(TokenType::kw_module)->getPosition();
    const auto module_name = ident();

    static_cast<void>(require_token(TokenType::semicolon));

    const auto decls = declarations();

    const StatementSequenceNode* statements = nullptr;
    if (scanner_->peekToken()->getType() == TokenType::kw_begin) {
        // just consume the token, we've checked it already.
        static_cast<void>(scanner_->nextToken());
        statements = statement_sequence();
    }

    static_cast<void>(require_token(TokenType::kw_end));

    const auto name_pos = scanner_->peekToken()->getPosition();
    const auto module_name2 = ident();
    if (module_name != module_name2) {
        logger_->error(name_pos, "Expected equal module names but got " + module_name + " and " +
                                     module_name2 + ".");
        exit(EXIT_FAILURE);
    }
    static_cast<void>(require_token(TokenType::period));

    return new ModuleNode(pos, module_name, decls, statements);
}

DeclarationsNode* Parser::declarations()
{
    auto constants = std::make_unique<ConstDeclList>();
    auto types = std::make_unique<TypeDeclList>();
    auto variables = std::make_unique<VarDeclList>();
    auto procedures = std::make_unique<ProcDeclList>();

    auto next = scanner_->peekToken();
    const auto pos = next->getPosition();

    if (next->getType() == TokenType::kw_const) {
        static_cast<void>(scanner_->nextToken()); // const keyword

        next = scanner_->peekToken();
        while (next->getType() == TokenType::const_ident) {
            constants->emplace_back(const_declaration());
            next = scanner_->peekToken();
        }
    }

    if (next->getType() == TokenType::kw_type) {
        static_cast<void>(scanner_->nextToken()); // type keyword

        next = scanner_->peekToken();
        while (next->getType() == TokenType::const_ident) {
            types->emplace_back(type_declaration());
            next = scanner_->peekToken();
        }
    }

    if (next->getType() == TokenType::kw_var) {
        static_cast<void>(scanner_->nextToken()); // var keyword

        next = scanner_->peekToken();
        while (next->getType() == TokenType::const_ident) {
            variables->emplace_back(var_declaration());
            next = scanner_->peekToken();
        }
    }

    while (next->getType() == TokenType::kw_procedure) {
        auto procDec = procedure_declaration();
        static_cast<void>(require_token(TokenType::semicolon));
        auto proc = std::unique_ptr<ProcedureDeclarationNode, ProcDeclDeleter>(procDec);
        procedures->push_back(std::move(proc));
        //
        next = scanner_->peekToken();
    }
    return new DeclarationsNode(pos, std::move(constants), std::move(variables), std::move(types),
                                std::move(procedures));
}

ConstantDeclarationNode* Parser::const_declaration()
{
    const auto pos = scanner_->peekToken()->getPosition();
    const auto name = ident();
    static_cast<void>(require_token(TokenType::op_eq));
    const auto value = expression();
    static_cast<void>(require_token(TokenType::semicolon));
    return new ConstantDeclarationNode(pos, name, value);
}

TypeDeclarationNode* Parser::type_declaration()
{
    const auto pos = scanner_->peekToken()->getPosition();
    const auto name = ident();
    static_cast<void>(require_token(TokenType::op_eq));
    const auto tp = type();
    static_cast<void>(require_token(TokenType::semicolon));
    return new TypeDeclarationNode(pos, name, tp);
}

VariableListNode* Parser::var_declaration()
{
    const auto pos = scanner_->peekToken()->getPosition();
    const auto names = ident_list();
    static_cast<void>(require_token(TokenType::colon));
    const auto tp = type();
    static_cast<void>(require_token(TokenType::semicolon));
    return createVariableList(pos, names, tp);
}

ProcedureDeclarationNode* Parser::procedure_declaration()
{
    // procedure heading
    const auto pos = require_token(TokenType::kw_procedure)->getPosition();
    const auto name = ident();

    FormalParameterList* params = nullptr;
    if (scanner_->peekToken()->getType() == TokenType::lparen) {
        params = formal_parameters();
    }
    static_cast<void>(require_token(TokenType::semicolon));

    // procedure body
    const auto decls = declarations();
    const StatementSequenceNode* statements = nullptr;
    if (scanner_->peekToken()->getType() == TokenType::kw_begin) {
        static_cast<void>(scanner_->nextToken());
        statements = statement_sequence();
    }

    static_cast<void>(require_token(TokenType::kw_end));
    const auto name2 = ident();

    if (name != name2) {
        logger_->error(pos,
                       "Expected equal procedure names but got " + name + " and " + name2 + ".");
        exit(EXIT_FAILURE);
    }

    return new ProcedureDeclarationNode(pos, name, params, decls, statements);
}

ExpressionNode* Parser::expression()
{
    const auto operand_1 = simple_expression();
    const auto maybe_operand = scanner_->peekToken()->getType();
    if (maybe_operand == TokenType::op_eq || maybe_operand == TokenType::op_neq ||
        maybe_operand == TokenType::op_lt || maybe_operand == TokenType::op_leq ||
        maybe_operand == TokenType::op_gt || maybe_operand == TokenType::op_geq) {
        auto op = scanner_->nextToken();
        const auto operand_2 = simple_expression();
        return evaluateBinaryExpression(operand_1, operand_2, std::move(op));
    }

    return operand_1;
}

ExpressionNode* Parser::simple_expression()
{
    ExpressionNode* operand_1;

    auto next = scanner_->peekToken()->getType();
    if (next == TokenType::op_plus || next == TokenType::op_minus) {
        auto op = scanner_->nextToken();
        const auto operand = term();
        operand_1 = evaluateUnaryExpression(operand, std::move(op));
    } else {
        operand_1 = term();
    }

    next = scanner_->peekToken()->getType();
    while (next == TokenType::op_plus || next == TokenType::op_minus || next == TokenType::op_or) {
        auto op = scanner_->nextToken();
        const auto operand_2 = term();
        operand_1 = evaluateBinaryExpression(operand_1, operand_2, std::move(op));
        next = scanner_->peekToken()->getType();
    }
    return operand_1;
}

ExpressionNode* Parser::term()
{
    auto operand_1 = factor();
    auto next = scanner_->peekToken()->getType();
    while (next == TokenType::op_times || next == TokenType::op_div || next == TokenType::op_mod ||
           next == TokenType::op_and) {
        auto op = scanner_->nextToken();
        const auto operand_2 = factor();
        operand_1 = evaluateBinaryExpression(operand_1, operand_2, std::move(op));
        next = scanner_->peekToken()->getType();
    }

    return operand_1;
}

ExpressionNode* Parser::factor()
{
    const auto next = scanner_->peekToken();
    const auto pos = next->getPosition();

    switch (next->getType()) {
    case TokenType::const_ident: {
        const auto name = ident();
        const auto sel = selector();

        // TODO: perform lookup in symbol table and return value if constant

        return new VariableReferenceNode(pos, name, sel);
    }
    case TokenType::const_number: {
        const auto token = scanner_->nextToken();
        const auto num_token = dynamic_cast<const NumberToken*>(token.get());
        assert(num_token != nullptr);
        const auto value = num_token->getValue();

        return new NumberConstantNode(pos, value);
    }
    case TokenType::const_string: {
        const auto token = scanner_->nextToken();
        const auto str_token = dynamic_cast<const StringToken*>(token.get());
        assert(str_token != nullptr);
        const auto value = str_token->getValue();

        return new StringConstantNode(pos, value);
    }
    case TokenType::lparen: {
        static_cast<void>(require_token(TokenType::lparen));
        const auto expr = expression();
        static_cast<void>(require_token(TokenType::rparen));
        return expr;
    }
    case TokenType::op_not: {
        auto op = require_token(TokenType::op_not);
        const auto value = factor();
        return evaluateUnaryExpression(value, std::move(op));
    }
    default:
        logger_->error(next->getPosition(), "Expected Identifier, Number, lparen or ~ but got " +
                                                (std::stringstream() << *next).str() + ".");
        exit(EXIT_FAILURE);
    }
}

TypeNode* Parser::type()
{
    const auto next = scanner_->peekToken();

    if (next->getType() == TokenType::const_ident) {
        const auto pos = next->getPosition();
        return new BasicTypeNode(pos, ident());
    }

    if (next->getType() == TokenType::kw_array) {
        return array_type();
    }

    if (next->getType() == TokenType::kw_record) {
        return record_type();
    }

    logger_->error(next->getPosition(), "Expected Identifier, ARRAY or RECORD but got " +
                                            (std::stringstream() << *next).str() + ".");
    exit(EXIT_FAILURE);
}

ArrayTypeNode* Parser::array_type()
{
    const auto pos = require_token(TokenType::kw_array)->getPosition();
    const auto arrayValue = expression();
    static_cast<void>(require_token(TokenType::kw_of));
    const auto arrayType = type();

    return new ArrayTypeNode(pos, arrayValue, arrayType);
}

RecordTypeNode* Parser::record_type()
{
    const auto pos = require_token(TokenType::kw_record)->getPosition();

    auto node = new RecordTypeNode(pos);
    node->addFields(field_list());

    while (scanner_->peekToken()->getType() == TokenType::semicolon) {
        static_cast<void>(require_token(TokenType::semicolon));
        node->addFields(field_list());
    }

    static_cast<void>(require_token(TokenType::kw_end));

    return node;
}

FieldListNode* Parser::field_list()
{
    if (scanner_->peekToken()->getType() != TokenType::const_ident) {
        return nullptr;
    }

    const auto identifiers = ident_list();
    static_cast<void>(require_token(TokenType::colon));
    const auto listType = type();

    return createFieldList(identifiers->getFilePos(), identifiers, listType);
}

IdentifierListNode* Parser::ident_list()
{
    const auto pos = scanner_->peekToken()->getPosition();
    auto node = new IdentifierListNode(pos, ident());

    while (scanner_->peekToken()->getType() == TokenType::comma) {
        static_cast<void>(scanner_->nextToken());
        node->addName(ident());
    }

    return node;
}

FormalParameterList* Parser::formal_parameters()
{
    const auto pos = require_token(TokenType::lparen)->getPosition();

    std::vector<std::unique_ptr<const ParameterListNode>> nodes;
    if (scanner_->peekToken()->getType() == TokenType::kw_var ||
        scanner_->peekToken()->getType() == TokenType::const_ident) {

        nodes.emplace_back(fp_section());

        while (scanner_->peekToken()->getType() == TokenType::semicolon) {
            static_cast<void>(scanner_->nextToken());

            nodes.emplace_back(fp_section());
        }
    }

    static_cast<void>(require_token(TokenType::rparen));

    return new FormalParameterList(pos, std::move(nodes));
}

ParameterListNode* Parser::fp_section()
{
    const auto next_token = scanner_->peekToken();
    const auto pos = next_token->getPosition();

    auto is_reference = false;
    if (next_token->getType() == TokenType::kw_var) {
        static_cast<void>(scanner_->nextToken());
        is_reference = true;
    }

    const auto names = ident_list();
    static_cast<void>(require_token(TokenType::colon));
    const auto list_type = type();

    return createParameterList(pos, names, list_type, is_reference);
}

StatementSequenceNode* Parser::statement_sequence()
{
    const auto node = new StatementSequenceNode(scanner_->peekToken()->getPosition());
    node->pushStatement(statement());
    while (scanner_->peekToken()->getType() == TokenType::semicolon) {
        static_cast<void>(scanner_->nextToken());
        node->pushStatement(statement());
    }
    return node;
}

StatementNode* Parser::statement()
{
    const auto next = scanner_->peekToken();
    if (next->getType() == TokenType::const_ident) {
        return procedure_call_or_assignment();
    }
    if (next->getType() == TokenType::kw_if) {
        return if_statement();
    }
    if (next->getType() == TokenType::kw_while) {
        return while_statement();
    }

    logger_->error(next->getPosition(), "Expected Identifier, IF or WHILE but got " +
                                            (std::stringstream() << *next).str() + ".");
    exit(EXIT_FAILURE);
}

AssignmentNode* Parser::assignment(const VariableReferenceNode* assignee)
{
    const auto first = require_token(TokenType::op_becomes);
    const auto expr = expression();

    return new AssignmentNode(first->getPosition(), assignee, expr);
}

ProcedureCallNode* Parser::procedure_call(const FilePos& pos, const std::string name)
{
    const auto next = scanner_->peekToken();
    const ActualParameterNode* params = nullptr;
    if (next->getType() == TokenType::lparen) {
        params = actual_parameters();
    }

    return new ProcedureCallNode(pos, name, params);
}

StatementNode* Parser::procedure_call_or_assignment()
{
    const auto pos = scanner_->peekToken()->getPosition();
    const auto id = ident();

    const auto nt = scanner_->peekToken()->getType();

    if (nt == TokenType::period || nt == TokenType::lbrack || nt == TokenType::op_becomes) {
        const auto sel = selector();
        auto base = new VariableReferenceNode(pos, id);
        base->setSelector(sel);
        return assignment(base);
    }

    return procedure_call(pos, id);
}

IfStatementNode* Parser::if_statement()
{
    // if statement
    auto pos = require_token(TokenType::kw_if)->getPosition();
    auto cond = expression();
    static_cast<void>(require_token(TokenType::kw_then));
    auto block = statement_sequence();

    const auto node = new IfStatementNode(pos, cond, block);
    // this is the next node where the else body must be filled
    auto nextNode = node;

    // elif statements
    while (scanner_->peekToken()->getType() == TokenType::kw_elsif) {
        pos = scanner_->nextToken()->getPosition();
        cond = expression();
        static_cast<void>(require_token(TokenType::kw_then));
        block = statement_sequence();

        const auto body = new StatementSequenceNode(pos);
        nextNode->setElseBody(body);
        nextNode = new IfStatementNode(pos, cond, block);
        body->pushStatement(nextNode);
    }

    // else statement
    if (scanner_->peekToken()->getType() == TokenType::kw_else) {
        static_cast<void>(scanner_->nextToken());
        block = statement_sequence();
        nextNode->setElseBody(block);
    }

    static_cast<void>(require_token(TokenType::kw_end));

    return node;
}

WhileStatementNode* Parser::while_statement()
{
    const auto token = require_token(TokenType::kw_while);
    const auto cond = expression();
    static_cast<void>(require_token(TokenType::kw_do));
    const auto body = statement_sequence();
    static_cast<void>(require_token(TokenType::kw_end));

    return new WhileStatementNode(token->getPosition(), cond, body);
}

ActualParameterNode* Parser::actual_parameters()
{
    ActualParameterNode* node = nullptr;

    static_cast<void>(require_token(TokenType::lparen));

    if (scanner_->peekToken()->getType() != TokenType::rparen) {
        node = new ActualParameterNode(expression());
        auto nextNode = node;

        while (scanner_->peekToken()->getType() == TokenType::comma) {
            static_cast<void>(scanner_->nextToken());

            auto next = new ActualParameterNode(expression());
            nextNode->setNext(next);
            nextNode = next;
        }
    }

    static_cast<void>(require_token(TokenType::rparen));

    return node;
}

SelectorNode* Parser::selector()
{
    SelectorNode* node = nullptr;
    SelectorNode* current_node = nullptr;
    auto next = scanner_->peekToken();
    while (true) {
        SelectorNode* next_node;
        if (next->getType() == TokenType::period) {
            const auto id_token = scanner_->nextToken();
            const auto name = ident();

            next_node = new FieldReferenceNode(id_token->getPosition(), name);
        } else if (next->getType() == TokenType::lbrack) {
            const auto openToken = scanner_->nextToken();
            const auto expr = expression();
            static_cast<void>(require_token(TokenType::rbrack));

            next_node = new ArrayReferenceNode(openToken->getPosition(), expr);
        } else {
            break;
        }

        if (node == nullptr) {
            node = next_node;
            current_node = next_node;
        } else {
            current_node->setNext(next_node);
        }

        next = scanner_->peekToken();
    }
    return node;
}

ExpressionNode* Parser::evaluateUnaryExpression(ExpressionNode* operand,
                                                const std::unique_ptr<const Token> op)
{
    assert(operand != nullptr);
    const auto unary_op = toUnaryOperator(op->getType());
    auto first = dynamic_cast<NumberConstantNode*>(operand);
    if (first != nullptr) {
        switch (unary_op) {
        case UnaryOperator::plus:
            first->setValue(+first->getValue());
            break;
        case UnaryOperator::minus:
            first->setValue(-first->getValue());
            break;
        case UnaryOperator::not:
            first->setValue(!first->getValue());
            break;
        default:
            std::cerr << "Invalid operator. This shouldn't be possible." << std::endl;
            std::terminate();
        }
        return first;
    }

    return new UnaryExpressionNode(op->getPosition(), unary_op, operand);
}

ExpressionNode* Parser::evaluateBinaryExpression(ExpressionNode* operand_1,
                                                 ExpressionNode* operand_2,
                                                 const std::unique_ptr<const Token> op)
{
    assert(operand_1 != nullptr);
    assert(operand_2 != nullptr);

    const auto bin_op = toBinaryOperator(op->getType());
    auto first = dynamic_cast<NumberConstantNode*>(operand_1);
    const auto second = dynamic_cast<NumberConstantNode*>(operand_2);
    if (first != nullptr && second != nullptr) {
        switch (bin_op) {
        case BinaryOperator::times:
            first->setValue(first->getValue() * second->getValue());
            break;
        case BinaryOperator::div:
            first->setValue(first->getValue() / second->getValue());
            break;
        case BinaryOperator::mod:
            first->setValue(first->getValue() % second->getValue());
            break;
        case BinaryOperator::logical_and:
            first->setValue(first->getValue() && second->getValue());
            break;
        case BinaryOperator::logical_or:
            first->setValue(first->getValue() || second->getValue());
            break;
        case BinaryOperator::plus:
            first->setValue(first->getValue() + second->getValue());
            break;
        case BinaryOperator::minus:
            first->setValue(first->getValue() - second->getValue());
            break;
        case BinaryOperator::eq:
            first->setValue(first->getValue() == second->getValue());
            break;
        case BinaryOperator::neq:
            first->setValue(first->getValue() != second->getValue());
            break;
        case BinaryOperator::lt:
            first->setValue(first->getValue() < second->getValue());
            break;
        case BinaryOperator::leq:
            first->setValue(first->getValue() <= second->getValue());
            break;
        case BinaryOperator::gt:
            first->setValue(first->getValue() > second->getValue());
            break;
        case BinaryOperator::geq:
            first->setValue(first->getValue() >= second->getValue());
            break;
        default:
            std::cerr << "Invalid operator. This shouldn't be possible." << std::endl;
            std::terminate();
        }

        delete second;
        return first;
    }

    return new BinaryExpressionNode(operand_1->getFilePos(), bin_op, operand_1, operand_2);
}
