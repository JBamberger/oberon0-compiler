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
#include "ast/ProcedureDeclarationList.h"
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
    const auto pos = scanner_->peekToken()->getPosition();
    const auto constDecls = const_declarations();
    const auto typeDecls = type_declarations();
    const auto varDecls = var_declarations();

    auto next = scanner_->peekToken();
    const auto procPos = next->getPosition();
    std::vector<std::unique_ptr<const ProcedureDeclarationNode>> procedureNodes;
    while (next->getType() == TokenType::kw_procedure) {
        const auto procDec = procedure_declaration();
        static_cast<void>(require_token(TokenType::semicolon));

        procedureNodes.emplace_back(procDec);

        next = scanner_->peekToken();
    }

    const auto procDecls = new ProcedureDeclarationList(procPos, std::move(procedureNodes));
    return new DeclarationsNode(pos, constDecls, varDecls, typeDecls, procDecls,
                                [](const ProcedureDeclarationList* l) { delete l; });
}

ConstantDeclarationList* Parser::const_declarations()
{
    auto next = scanner_->peekToken();
    const auto pos = next->getPosition();
    std::vector<std::unique_ptr<const ConstantDeclarationNode>> nodes;

    if (next->getType() != TokenType::kw_const) {
        return new ConstantDeclarationList(pos, std::move(nodes));
    }

    static_cast<void>(scanner_->nextToken()); // const keyword

    next = scanner_->peekToken();
    while (next->getType() == TokenType::const_ident) {
        const auto decPos = next->getPosition();
        const auto name = ident();
        static_cast<void>(require_token(TokenType::op_eq));
        const auto value = expression();
        static_cast<void>(require_token(TokenType::semicolon));

        nodes.emplace_back(new ConstantDeclarationNode(decPos, name, value));

        next = scanner_->peekToken();
    }

    return new ConstantDeclarationList(pos, std::move(nodes));
}

TypeDeclarationList* Parser::type_declarations()
{

    auto next = scanner_->peekToken();
    const auto pos = next->getPosition();
    std::vector<std::unique_ptr<const TypeDeclarationNode>> nodes;

    if (next->getType() != TokenType::kw_type) {
        return new TypeDeclarationList(pos, std::move(nodes));
    }

    static_cast<void>(scanner_->nextToken()); // type keyword

    next = scanner_->peekToken();
    while (next->getType() == TokenType::const_ident) {
        const auto typePos = next->getPosition();
        const auto name = ident();
        static_cast<void>(require_token(TokenType::op_eq));
        const auto tp = type();
        static_cast<void>(require_token(TokenType::semicolon));

        nodes.emplace_back(new TypeDeclarationNode(typePos, name, tp));

        next = scanner_->peekToken();
    }
    return new TypeDeclarationList(pos, std::move(nodes));
}

VariableDeclarationList* Parser::var_declarations()
{
    auto next = scanner_->peekToken();
    const auto pos = next->getPosition();
    std::vector<std::unique_ptr<const VariableListNode>> nodes;

    if (next->getType() != TokenType::kw_var) {
        return new VariableDeclarationList(pos, std::move(nodes));
    }

    static_cast<void>(scanner_->nextToken()); // var keyword

    next = scanner_->peekToken();
    while (next->getType() == TokenType::const_ident) {
        const auto varPos = next->getPosition();
        const auto names = ident_list();
        static_cast<void>(require_token(TokenType::colon));
        const auto tp = type();
        static_cast<void>(require_token(TokenType::semicolon));

        nodes.emplace_back(createVariableList(varPos, names, tp));

        next = scanner_->peekToken();
    }
    return new VariableDeclarationList(pos, std::move(nodes));
}

ProcedureDeclarationNode* Parser::procedure_declaration()
{
    // procedure heading
    const auto pos = require_token(TokenType::kw_procedure)->getPosition();
    const auto name = ident();

    const FormalParameterList* params = nullptr;
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
    const auto node = simple_expression();

    BinaryOperator op;
    const auto nt = scanner_->peekToken()->getType();
    if (nt == TokenType::op_eq) {
        op = BinaryOperator::eq;
    } else if (nt == TokenType::op_neq) {
        op = BinaryOperator::neq;
    } else if (nt == TokenType::op_lt) {
        op = BinaryOperator::lt;
    } else if (nt == TokenType::op_leq) {
        op = BinaryOperator::leq;
    } else if (nt == TokenType::op_gt) {
        op = BinaryOperator::gt;
    } else if (nt == TokenType::op_geq) {
        op = BinaryOperator::geq;
    } else {
        return node;
    }

    const auto next = scanner_->nextToken();
    const auto second = simple_expression();
    return new BinaryExpressionNode(next->getPosition(), op, node, second);
}

ExpressionNode* Parser::simple_expression()
{
    ExpressionNode* node;

    auto next = scanner_->peekToken()->getType();
    if (next == TokenType::op_plus || next == TokenType::op_minus) {
        const auto prefix = scanner_->nextToken();
        const auto operand = term();
        const auto op = next == TokenType::op_plus ? UnaryOperator::plus : UnaryOperator::minus;
        node = new UnaryExpressionNode(prefix->getPosition(), op, operand);
    } else {
        node = term();
    }

    next = scanner_->peekToken()->getType();
    while (true) {
        BinaryOperator op;
        if (next == TokenType::op_plus) {
            op = BinaryOperator::plus;
        } else if (next == TokenType::op_minus) {
            op = BinaryOperator::minus;
        } else if (next == TokenType::op_or) {
            op = BinaryOperator::logical_or;
        } else {
            break;
        }

        const auto nextOp = scanner_->nextToken();
        const auto otherNode = term();
        node = new BinaryExpressionNode(nextOp->getPosition(), op, node, otherNode);
        next = scanner_->peekToken()->getType();
    }
    return node;
}

ExpressionNode* Parser::term()
{
    auto operand_1 = factor();

    auto next = scanner_->peekToken()->getType();
    while (true) {
        BinaryOperator op;
        if (next == TokenType::op_times) {
            op = BinaryOperator::times;
        } else if (next == TokenType::op_div) {
            op = BinaryOperator::div;
        } else if (next == TokenType::op_mod) {
            op = BinaryOperator::mod;
        } else if (next == TokenType::op_and) {
            op = BinaryOperator::logical_and;
        } else {
            break;
        }

        const auto operation = scanner_->nextToken();
        const auto operand_2 = factor();

        operand_1 = new BinaryExpressionNode(operation->getPosition(), op, operand_1, operand_2);

        next = scanner_->peekToken()->getType();
    }

    return operand_1;
}

ExpressionNode* Parser::factor()
{
    const auto next = scanner_->peekToken();
    const auto pos = next->getPosition();

    // variable access
    if (next->getType() == TokenType::const_ident) {
        const auto name = ident();
        const auto sel = selector();
        const auto variable = new VariableReferenceNode(pos, name);
        variable->setSelector(sel);
        return variable;
    }

    // number constant
    if (next->getType() == TokenType::const_number) {
        const auto num = scanner_->nextToken();
        const auto numToken = dynamic_cast<const NumberToken*>(num.get());
        assert(numToken != nullptr);
        const auto value = numToken->getValue();

        return new NumberConstantNode(pos, value);
    }

    // string constant
    if (next->getType() == TokenType::const_string) {
        const auto token = scanner_->nextToken();
        const auto strToken = dynamic_cast<const StringToken*>(token.get());
        assert(strToken != nullptr);
        const auto value = strToken->getValue();

        return new StringConstantNode(pos, value);
    }

    // TODO: bool constant here

    // nested expression
    if (next->getType() == TokenType::lparen) {
        static_cast<void>(require_token(TokenType::lparen));
        const auto expr = expression();
        static_cast<void>(require_token(TokenType::rparen));
        return expr;
    }

    // negation
    if (next->getType() == TokenType::op_not) {
        const auto op = require_token(TokenType::op_not);
        const auto value = factor();

        return new UnaryExpressionNode(pos, UnaryOperator::not, value);
    }

    logger_->error(next->getPosition(), "Expected Identifier, Number, lparen or ~ but got " +
                                            (std::stringstream() << *next).str() + ".");
    exit(EXIT_FAILURE);
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
