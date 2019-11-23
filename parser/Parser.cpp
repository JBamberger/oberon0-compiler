//
// Created by Michael Grossniklaus on 11/20/18.
//

#include "Parser.h"
#include <IdentToken.h>
#include <NumberToken.h>
#include <iostream>
#include <sstream>
#include <cassert>

Parser::Parser(Scanner* scanner, Logger* logger) : scanner_(scanner), logger_(logger) {}

Parser::~Parser() = default;

/***
Grammar:

ident = letter {letter | digit}.

integer = digit {digit}.

selector = {"." ident | "[" expression "]"}.

number = integer.

factor = ident selector | number | "(" expression ")" | "~" factor.

term = factor {("*" | "DIV" | "MOD" | "&") factor}.

SimpleExpression = ["+"|"-"] term {("+"|"-" | "OR") term}.

expression = SimpleExpression [("=" | "#" | "<" | "<=" | ">" | ">=") SimpleExpression].

assignment = ident selector ":=" expression.

ActualParameters = "(" [expression {"," expression}] ")" .

ProcedureCall = ident selector [ActualParameters].

IfStatement = "IF" expression "THEN" StatementSequence
              {"ELSIF" expression "THEN" StatementSequence}
              ["ELSE" StatementSequence]
              "END".

WhileStatement = "WHILE" expression "DO" StatementSequence "END".

statement = [assignment | ProcedureCall | IfStatement | WhileStatement].

StatementSequence = statement {";" statement}.

IdentList = ident {"," ident}.

ArrayType = "ARRAY" expression "OF" type.

FieldList = [IdentList ":" type].

RecordType = "RECORD" FieldList {";" FieldList} "END".

type = ident | ArrayType | RecordType.

FPSection = ["VAR"] IdentList ":" type.

FormalParameters = "(" [FPSection {";" FPSection}] ")".

ProcedureHeading = "PROCEDURE" ident [FormalParameters].

ProcedureBody = declarations ["BEGIN" StatementSequence] "END" ident.

ProcedureDeclaration = ProcedureHeading ";" ProcedureBody.

declarations = ["CONST" {ident "=" expression ";"}]
               ["TYPE" {ident "=" type ";"}]
               ["VAR" {IdentList ":" type ";"}]
               {ProcedureDeclaration ";"}.

module = "MODULE" ident ";"
         declarations ["BEGIN" StatementSequence]
         "END" ident "." .
***/

inline std::string type_str(const TokenType& t) { return (std::stringstream() << t).str(); }

std::unique_ptr<const Node> Parser::parse() { return std::unique_ptr<const Node>(module()); }

std::unique_ptr<const Token> Parser::require_token(const TokenType& type)
{
    auto token = scanner_->nextToken();
    if (token->getType() != type) {
        const auto msg = "Expected " + type_str(type) + " but got " +
                         (std::stringstream() << *token).str() + ".";
        logger_->error(token->getPosition(), msg);
        // TODO: terminate
    }
    return token;
}

std::string Parser::ident()
{
	auto token = require_token(TokenType::const_ident);
    const auto ident_token = dynamic_cast<const IdentToken*>(token.get());
    assert(ident_token != nullptr);
	
    return ident_token->getValue();
}
const Node* Parser::module()
{
    require_token(TokenType::kw_module);
    const auto moduleName = ident();
    require_token(TokenType::semicolon);
    const auto decls = declarations();

    if (scanner_->peekToken()->getType() == TokenType::kw_begin) {
        scanner_->nextToken(); // just consume the token, we've checked it already.
        const auto statements = statement_sequence();
    }

    require_token(TokenType::kw_end);

    const auto namePos = scanner_->peekToken()->getPosition();
    const auto moduleName2 = ident();
    if (moduleName != moduleName2) {
        logger_->error(namePos, "Expected equal module names but got " + moduleName + " and " +
                                    moduleName2 + ".");
        // TODO: terminate
    }
    require_token(TokenType::period);

    // TODO: const auto node =  std::make_unique<ModuleNode>(moduleName, decls,
    // statements?);
    return nullptr;
}

const Node* Parser::declarations()
{
    const auto constDecls = const_declarations();
    const auto typeDecls = type_declarations();
    const auto varDecls = var_declarations();

    auto next = scanner_->peekToken();
    while (next->getType() == TokenType::kw_procedure) {
        const auto procDec = procedure_declaration();
        require_token(TokenType::semicolon);
        // TODO: add to tlist
        next = scanner_->peekToken();
    }

    // TODO: add std::make_unique<DeclarationsNode>(constDecls, typeDecls,
    // varDecls, procDecl);
    return nullptr;
}

const Node* Parser::const_declarations()
{
    auto next = scanner_->peekToken();

    if (next->getType() == TokenType::kw_const) {
        scanner_->nextToken(); // consume the const keyword
        next = scanner_->peekToken();
        while (next->getType() == TokenType::const_ident) {
            const auto name = ident();
            require_token(TokenType::op_eq);
            const auto expr = expression();
            require_token(TokenType::semicolon);
            // TODO: add std::make_unique<ConstDeclNode>(name, expr);
            next = scanner_->peekToken();
        }
    }
    // TODO: list
    return nullptr;
}

const Node* Parser::type_declarations()
{
    auto next = scanner_->peekToken();

    if (next->getType() == TokenType::kw_type) {
        scanner_->nextToken();
        next = scanner_->peekToken();
        while (next->getType() == TokenType::const_ident) {
            const auto name = ident();
            require_token(TokenType::op_eq);
            const auto tp = type();
            require_token(TokenType::semicolon);
            // TODO: add std::make_unique<TypeDeclNode>(name, tp);
            next = scanner_->peekToken();
        }
    }
    // TODO: list
    return nullptr;
}

const Node* Parser::var_declarations()
{
    auto next = scanner_->peekToken();

    if (next->getType() == TokenType::kw_var) {
        scanner_->nextToken();
        next = scanner_->peekToken();
        while (next->getType() == TokenType::const_ident) {
            const auto identList = ident_list();
            require_token(TokenType::colon);
            const auto tp = type();
            require_token(TokenType::semicolon);
            // TODO: add std::make_unique<VarDeclNode>(identList, tp);
            next = scanner_->peekToken();
        }
    }
    // TODO: list
    return nullptr;
}

const Node* Parser::procedure_declaration()
{
    const auto heading = procedure_heading();
    require_token(TokenType::semicolon);
    const auto body = procedure_body();
    // TODO: std::make_unique<ProcedureDeclarationNode>(heading, body);
    return nullptr;
}

const Node* Parser::procedure_heading()
{
    require_token(TokenType::kw_procedure);
    const auto name = ident();
    if (scanner_->peekToken()->getType() == TokenType::lparen) {
        const auto params = formal_parameters();
    }
    // TODO: std::make_unique<ProcedureHeadingNode>(name, params);
    return nullptr;
}

const Node* Parser::procedure_body()
{
    const auto decls = declarations();
    if (scanner_->peekToken()->getType() == TokenType::kw_begin) {
        scanner_->nextToken();
        const auto seq = statement_sequence();
    }
    require_token(TokenType::kw_end);
    const auto name = ident();
    // TODO: std::make_unique<ProcedureBodyNode>(name, decls, seq);
    return nullptr;
}

const Node* Parser::expression()
{
    const auto first = simple_expression();
    bool readSecond = false;
    switch (scanner_->peekToken()->getType()) {
    case TokenType::op_eq:
    case TokenType::op_neq:
    case TokenType::op_leq:
    case TokenType::op_lt:
    case TokenType::op_geq:
    case TokenType::op_gt:
        readSecond = true;
        break;
    default:
        break;
    }
    if (readSecond) {
        const auto type = scanner_->nextToken()->getType();
        const auto second = simple_expression();
        // TODO: std::make_unique<ComparisonNode>(first, second, operation);
    }
    // TODO: first;
    return nullptr;
}

const Node* Parser::simple_expression()
{
    auto next = scanner_->peekToken()->getType();
    if (next == TokenType::op_plus || next == TokenType::op_minus) {
        const auto prefix = scanner_->nextToken();
    }
    const auto first = term();
    next = scanner_->peekToken()->getType();
    while (next == TokenType::op_plus || next == TokenType::op_minus || next == TokenType::op_or) {
        const auto nextOp = scanner_->nextToken();
        const auto nextTerm = term();
        // TODO: add to list BinOpNode(nextOp, nextTerm, other);
        next = scanner_->peekToken()->getType();
    }
    // TODO: list
    return nullptr;
}

const Node* Parser::term()
{
    const auto first = factor();
    auto next = scanner_->peekToken()->getType();
    while (next == TokenType::op_times || next == TokenType::op_div || next == TokenType::op_mod ||
           next == TokenType::op_and) {
        const auto operation = scanner_->nextToken();
        const auto operand = factor();
        // TODO: add to list BinNode(op, operand, other);
        next = scanner_->peekToken()->getType();
    }
    // TODO: list
    return nullptr;
}

const Node* Parser::factor()
{
    auto next = scanner_->peekToken();
    if (next->getType() == TokenType::const_ident) {
        const auto name = ident();
        const auto sel = selector();
        // TODO: std::make_unique<IdFactorNode>(name, sel);
    } else if (next->getType() == TokenType::const_number) {
        const auto num = scanner_->nextToken();
        const auto numToken = dynamic_cast<const NumberToken*>(num.get());
        assert(numToken != nullptr);
    	
        const auto number = numToken->getValue();
        // TODO: std::make_unique<NumberNode>(number);
    } else if (next->getType() == TokenType::lparen) {
        require_token(TokenType::lparen);
        const auto expr = expression();
        require_token(TokenType::rparen);
        // TODO: expr
    } else if (next->getType() == TokenType::op_not) {
        require_token(TokenType::op_not);
        const auto f = factor();
        // TODO: f
    } else {
        logger_->error(next->getPosition(), "Expected Identifier, Number, lparen or ~ but got " +
                                                (std::stringstream() << *next).str() + ".");
        // TODO: terminate
    }

    return nullptr;
}

const Node* Parser::type()
{
    const auto next = scanner_->peekToken();
    if (next->getType() == TokenType::const_ident) {
        const auto name = ident();
        // TODO: std::make_unique<ReferenceTypeNode>(name);
    } else if (next->getType() == TokenType::kw_array) {
        const auto arr = array_type();
        // TODO: arr;
    } else if (next->getType() == TokenType::kw_record) {
        const auto rec = record_type();
        // TODO: rec;
    } else {
        logger_->error(next->getPosition(), "Expected Identifier, ARRAY or RECORD but got " +
                                                (std::stringstream() << *next).str() + ".");
        // TODO: terminate
    }
    return nullptr;
}

const Node* Parser::array_type()
{
    require_token(TokenType::kw_array);
    const auto expr = expression();
    require_token(TokenType::kw_of);
    const auto t = type();
    // TODO: std::make_unique<ArrayTypeNode>(expr, t);
    return nullptr;
}

const Node* Parser::record_type()
{
    require_token(TokenType::kw_record);
    const auto fields = field_list();
    while (scanner_->peekToken()->getType() == TokenType::semicolon) {
        require_token(TokenType::semicolon);
        const auto moreFields = field_list();
        // TODO: add fields
    }
    require_token(TokenType::kw_end);
    // TODO: std::make_unique<RecordNode>(fields...);
    return nullptr;
}

const Node* Parser::field_list()
{
    if (scanner_->peekToken()->getType() == TokenType::const_ident) {
        const auto idList = ident_list();
        require_token(TokenType::colon);
        const auto t = type();
        // TODO: std::make_unique<FieldListNode>(idList, t);
    }

    return nullptr;
}

const Node* Parser::ident_list()
{
    const auto first = ident();
    while (scanner_->peekToken()->getType() == TokenType::comma) {
        scanner_->nextToken();
        const auto next = ident();
        // TODO: add to list
    }
    // TODO: std::make_unique<IdentifierListNode>(list);
    return nullptr;
}

const Node* Parser::formal_parameters()
{
    require_token(TokenType::lparen);
    if (scanner_->peekToken()->getType() == TokenType::kw_var ||
        scanner_->peekToken()->getType() == TokenType::const_ident) {
        const auto first = fp_section();
        // TODO: add to list
        while (scanner_->peekToken()->getType() == TokenType::semicolon) {
            scanner_->nextToken();
            const auto next = fp_section();
            // TODO: add to list
        }
    }
    require_token(TokenType::rparen);
    // TODO: std::make_unique<FormalParameterListNode>(list);
    return nullptr;
}

const Node* Parser::fp_section()
{
    bool isVar = false;
    if (scanner_->peekToken()->getType() == TokenType::kw_var) {
        scanner_->nextToken();
        isVar = true;
    }
    const auto idList = ident_list();
    require_token(TokenType::colon);
    const auto t = type();
    // TODO: std::make_shared<FormalParameterNode>(isVar, idList, t);
    return nullptr;
}

const Node* Parser::statement_sequence()
{
    const auto first = statement();
    // TODO: add to list
    while (scanner_->peekToken()->getType() == TokenType::semicolon) {
        scanner_->nextToken();
        const auto next = statement();
        // TODO: add to list
    }
    // std::make_unique<StatementListNode>(list);
    return nullptr;
}

const Node* Parser::statement()
{
    auto next = scanner_->peekToken();
    if (next->getType() == TokenType::const_ident) {
        const auto id = ident();
        const auto sel = selector();
        next = scanner_->peekToken();
        if (next->getType() == TokenType::op_becomes) {
            scanner_->nextToken();
            const auto expr = expression();
            // TODO: std::make_unique<AssignmentNode>(id, sel, expr);
        } else {
            if (next->getType() == TokenType::lparen) {
                const auto params = actual_parameters();
            }
            // TODO: std::make_unique<ProcedureCallNode(id, sel, params);
        }
    } else if (next->getType() == TokenType::kw_if) {
        return if_statement();
    } else if (next->getType() == TokenType::kw_while) {
        return while_statement();
    } else {
        logger_->error(next->getPosition(), "Expected Identifier, IF or WHILE but got " +
                                                (std::stringstream() << *next).str() + ".");
        // TODO: terminate
    }
    return nullptr;
}

const Node* Parser::if_statement()
{
    require_token(TokenType::kw_if);
    const auto ifCondition = expression();
    require_token(TokenType::kw_then);
    const auto ifBlock = statement_sequence();
    // TODO: add if branch
    while (scanner_->peekToken()->getType() == TokenType::kw_elsif) {
        scanner_->nextToken();
        const auto cond = expression();
        require_token(TokenType::kw_then);
        const auto blk = statement_sequence();
        // TODO: add to list
    }
    if (scanner_->peekToken()->getType() == TokenType::kw_else) {
        scanner_->nextToken();
        const auto blk = statement_sequence();
        // TODO: add else branch
    }

    require_token(TokenType::kw_end);
    // TODO: std::make_unique<ConditionalNode>(ifCondition, ifBlock, condList,
    // blkList, else
    return nullptr;
}

const Node* Parser::while_statement()
{
    require_token(TokenType::kw_while);
    const auto cond = expression();
    require_token(TokenType::kw_do);
    const auto body = statement_sequence();
    require_token(TokenType::kw_end);
    // TODO: std::make_unique<WhileNode>(cond, body);
    return nullptr;
}

const Node* Parser::actual_parameters()
{
    require_token(TokenType::lparen);
    if (scanner_->peekToken()->getType() != TokenType::rparen) {
        const auto first = expression();
        while (scanner_->peekToken()->getType() == TokenType::comma) {
            scanner_->nextToken();
            const auto next = expression();
            // TODO: add to list
        }
    }

    require_token(TokenType::rparen);
    // TODO: std::make_unique<ParamListNode>(list);
    return nullptr;
}

const Node* Parser::selector()
{
    auto next = scanner_->peekToken();
    while (true) {
        if (next->getType() == TokenType::period) {
            scanner_->nextToken();
            const auto id = ident();
            // TODO: add MemberNode(id);
        } else if (next->getType() == TokenType::lbrack) {
            scanner_->nextToken();
            const auto expr = expression();
            require_token(TokenType::rbrack);
            // TODO: add SelectorNode(expr);
        } else {
            break;
        }
    }
    // TODO: return list
    return nullptr;
}
