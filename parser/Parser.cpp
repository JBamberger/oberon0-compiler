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

std::unique_ptr<Node> Parser::parse() { return module(); }

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

std::unique_ptr<ModuleNode> Parser::module()
{
    const auto pos = require_token(TokenType::kw_module)->getPosition();
    const auto module_name = ident();

    auto module_node = std::make_unique<ModuleNode>(pos, module_name);
    current_scope_ = module_node->getScope();

    static_cast<void>(require_token(TokenType::semicolon));

    declarations(module_node.get());

    if (scanner_->peekToken()->getType() == TokenType::kw_begin) {
        // just consume the token, we've checked it already.
        static_cast<void>(scanner_->nextToken());
        statement_sequence(module_node->getStatements().get());
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
            block->getConstants()->push_back(const_declaration());
            next = scanner_->peekToken();
        }
    }

    if (next->getType() == TokenType::kw_type) {
        static_cast<void>(scanner_->nextToken()); // type keyword

        next = scanner_->peekToken();
        while (next->getType() == TokenType::const_ident) {
            block->getTypes()->push_back(type_declaration());
            next = scanner_->peekToken();
        }
    }

    if (next->getType() == TokenType::kw_var) {
        static_cast<void>(scanner_->nextToken()); // var keyword

        next = scanner_->peekToken();
        while (next->getType() == TokenType::const_ident) {
            block->getVariables()->push_back(var_declaration());
            next = scanner_->peekToken();
        }
    }

    while (next->getType() == TokenType::kw_procedure) {
        block->getProcedures()->push_back(procedure_declaration());
        static_cast<void>(require_token(TokenType::semicolon));
        next = scanner_->peekToken();
    }
}

std::unique_ptr<ConstantDeclarationNode> Parser::const_declaration()
{
    const auto pos = scanner_->peekToken()->getPosition();
    const auto name = ident();
    static_cast<void>(require_token(TokenType::op_eq));
    auto value = expression();
    static_cast<void>(require_token(TokenType::semicolon));
    return std::make_unique<ConstantDeclarationNode>(pos, name, std::move(value));
}

std::unique_ptr<TypeDeclarationNode> Parser::type_declaration()
{
    const auto pos = scanner_->peekToken()->getPosition();
    const auto name = ident();
    static_cast<void>(require_token(TokenType::op_eq));
    auto tp = type();
    static_cast<void>(require_token(TokenType::semicolon));
    return std::make_unique<TypeDeclarationNode>(pos, name, std::move(tp));
}

std::unique_ptr<VariableListNode> Parser::var_declaration()
{
    const auto pos = scanner_->peekToken()->getPosition();
    const auto names = ident_list();
    static_cast<void>(require_token(TokenType::colon));
    auto tp = type();
    static_cast<void>(require_token(TokenType::semicolon));
    return std::unique_ptr<VariableListNode>(createVariableList(pos, names, std::move(tp)));
}

std::unique_ptr<ProcedureDeclarationNode> Parser::procedure_declaration()
{
    // procedure heading
    const auto pos = require_token(TokenType::kw_procedure)->getPosition();
    const auto name = ident();

    auto proc_node = std::make_unique<ProcedureDeclarationNode>(pos, name, current_scope_);
    current_scope_ = proc_node->getScope();

    if (scanner_->peekToken()->getType() == TokenType::lparen) {
        formal_parameters(proc_node.get());
    }
    static_cast<void>(require_token(TokenType::semicolon));

    // procedure body
    declarations(proc_node.get());

    if (scanner_->peekToken()->getType() == TokenType::kw_begin) {
        static_cast<void>(scanner_->nextToken());
        statement_sequence(proc_node->getStatements().get());
    }

    static_cast<void>(require_token(TokenType::kw_end));
    const auto name2 = ident();

    if (name != name2) {
        logger_->error(pos,
                       "Expected equal procedure names but got " + name + " and " + name2 + ".");
        exit(EXIT_FAILURE);
    }

    current_scope_ = proc_node->getScope()->getParent();
    return proc_node;
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
        const auto name = ident();
        auto sel = selector();

        // TODO: perform lookup in symbol table and return value if constant

        return std::make_unique<VariableReferenceNode>(pos, name, std::move(sel));
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

std::unique_ptr<TypeNode> Parser::type()
{
    const auto next = scanner_->peekToken();

    if (next->getType() == TokenType::const_ident) {
        const auto pos = next->getPosition();
        return std::make_unique<BasicTypeNode>(pos, ident());
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

std::unique_ptr<ArrayTypeNode> Parser::array_type()
{
    const auto pos = require_token(TokenType::kw_array)->getPosition();
    auto arrayValue = expression();
    static_cast<void>(require_token(TokenType::kw_of));
    auto arrayType = type();

    return std::make_unique<ArrayTypeNode>(pos, std::move(arrayValue), std::move(arrayType));
}

std::unique_ptr<RecordTypeNode> Parser::record_type()
{
    const auto pos = require_token(TokenType::kw_record)->getPosition();

    auto node = std::make_unique<RecordTypeNode>(pos, current_scope_);
    current_scope_ = node->getScope();
    field_list(node.get());

    while (scanner_->peekToken()->getType() == TokenType::semicolon) {
        static_cast<void>(require_token(TokenType::semicolon));
        field_list(node.get());
    }

    static_cast<void>(require_token(TokenType::kw_end));

    current_scope_ = node->getScope()->getParent(); // reset scope
    return node;
}

void Parser::field_list(RecordTypeNode* rec_decl)
{
    if (scanner_->peekToken()->getType() != TokenType::const_ident) {
        return;
    }
    const auto names = ident_list();
    static_cast<void>(require_token(TokenType::colon));
    auto list_type = std::shared_ptr<TypeNode>(type());

    for (const auto& name : names->getNames()) {
        auto type_ref = std::make_unique<TypeReferenceNode>(list_type->getFilePos(), list_type);
        rec_decl->getMembers()->push_back(
            std::make_unique<FieldDeclarationNode>(names->getFilePos(), name, std::move(type_ref)));
    }
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

void Parser::formal_parameters(ProcedureDeclarationNode* proc_decl)
{
    const auto pos = require_token(TokenType::lparen)->getPosition();

    if (scanner_->peekToken()->getType() == TokenType::kw_var ||
        scanner_->peekToken()->getType() == TokenType::const_ident) {

        fp_section(proc_decl);

        while (scanner_->peekToken()->getType() == TokenType::semicolon) {
            static_cast<void>(scanner_->nextToken());

            fp_section(proc_decl);
        }
    }

    static_cast<void>(require_token(TokenType::rparen));
}

void Parser::fp_section(ProcedureDeclarationNode* proc_decl)
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
    auto list_type = std::shared_ptr<TypeNode>(type());

    for (const auto& name : names->getNames()) {
        auto type_ref = std::make_unique<TypeReferenceNode>(list_type->getFilePos(), list_type);
        proc_decl->getParams()->push_back(std::make_unique<ParameterDeclarationNode>(
            pos, name, std::move(type_ref), is_reference));
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

std::unique_ptr<AssignmentNode> Parser::assignment(std::unique_ptr<VariableReferenceNode> assignee)
{
    const auto first = require_token(TokenType::op_becomes);
    auto expr = expression();

    return std::make_unique<AssignmentNode>(first->getPosition(), std::move(assignee),
                                            std::move(expr));
}

std::unique_ptr<ProcedureCallNode> Parser::procedure_call(const FilePos& pos,
                                                          const std::string name)
{
    const auto next = scanner_->peekToken();
    auto proc = std::make_unique<ProcedureCallNode>(pos, name);
    if (next->getType() == TokenType::lparen) {
        actual_parameters(proc->getParameters().get());
    }

    return proc;
}

std::unique_ptr<StatementNode> Parser::procedure_call_or_assignment()
{
    const auto pos = scanner_->peekToken()->getPosition();
    const auto id = ident();

    const auto nt = scanner_->peekToken()->getType();

    if (nt == TokenType::period || nt == TokenType::lbrack || nt == TokenType::op_becomes) {
        auto sel = selector();
        auto base = std::make_unique<VariableReferenceNode>(pos, id);
        base->setSelector(std::move(sel));
        return assignment(std::move(base));
    }

    return procedure_call(pos, id);
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

void Parser::actual_parameters(std::vector<std::unique_ptr<ActualParameterNode>>* params)
{
    static_cast<void>(require_token(TokenType::lparen));

    if (scanner_->peekToken()->getType() != TokenType::rparen) {
        params->push_back(std::make_unique<ActualParameterNode>(expression()));

        while (scanner_->peekToken()->getType() == TokenType::comma) {
            static_cast<void>(scanner_->nextToken());

            params->push_back(std::make_unique<ActualParameterNode>(expression()));
        }
    }

    static_cast<void>(require_token(TokenType::rparen));
}

std::unique_ptr<SelectorNode> Parser::selector()
{
    std::unique_ptr<SelectorNode> node = nullptr;
    SelectorNode* current_node = nullptr;
    auto next = scanner_->peekToken();
    while (true) {
        std::unique_ptr<SelectorNode> next_node;
        if (next->getType() == TokenType::period) {
            const auto id_token = scanner_->nextToken();
            const auto name = ident();

            next_node = std::make_unique<FieldReferenceNode>(id_token->getPosition(), name);
        } else if (next->getType() == TokenType::lbrack) {
            const auto open_token = scanner_->nextToken();
            next_node =
                std::make_unique<ArrayReferenceNode>(open_token->getPosition(), expression());
            static_cast<void>(require_token(TokenType::rbrack));
        } else {
            break;
        }

        if (node == nullptr) {
            node = std::move(next_node);
            current_node = node.get();
        } else {
            current_node->setNext(std::move(next_node));
        }

        next = scanner_->peekToken();
    }
    return node;
}

std::unique_ptr<ExpressionNode>
Parser::evaluateUnaryExpression(std::unique_ptr<ExpressionNode> operand,
                                const std::unique_ptr<const Token> op)
{
    assert(operand != nullptr);
    const auto unary_op = toUnaryOperator(op->getType());
    auto first = dynamic_cast<NumberConstantNode*>(operand.get());
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
        return operand;
    }

    return std::make_unique<UnaryExpressionNode>(op->getPosition(), unary_op, std::move(operand));
}

std::unique_ptr<ExpressionNode>
Parser::evaluateBinaryExpression(std::unique_ptr<ExpressionNode> operand_1,
                                 std::unique_ptr<ExpressionNode> operand_2,
                                 const std::unique_ptr<const Token> op)
{
    assert(operand_1 != nullptr);
    assert(operand_2 != nullptr);

    const auto bin_op = toBinaryOperator(op->getType());
    const auto first = dynamic_cast<NumberConstantNode*>(operand_1.get());
    const auto second = dynamic_cast<NumberConstantNode*>(operand_2.get());
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

        return operand_1;
    }

    return std::make_unique<BinaryExpressionNode>(operand_1->getFilePos(), bin_op,
                                                  std::move(operand_1), std::move(operand_2));
}
