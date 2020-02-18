/**
 * Copyright 2020 Jannik Bamberger
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "ArrayTypeNode.h"
#include "AssignmentNode.h"
#include "ExpressionNode.h"
#include "Identifier.h"
#include "IfStatementNode.h"
#include "ModuleNode.h"
#include "Node.h"
#include "ParseException.h"
#include "ProcedureCallNode.h"
#include "ProcedureDeclarationNode.h"
#include "RecordTypeNode.h"
#include "Scanner.h"
#include "TypeNode.h"
#include "VariableDeclarationNode.h"
#include "WhileStatementNode.h"
#include <Operator.h>

enum class OperatorType : char;
enum class BinaryOperator : char;

class Parser {

    std::shared_ptr<Scanner> scanner_;
    std::shared_ptr<Logger> logger_;

    std::unordered_map<std::string, std::unique_ptr<TypeNode>> types_;
    std::shared_ptr<Scope> current_scope_;

    TypeNode* integer_ = nullptr;
    TypeNode* boolean_ = nullptr;
    TypeNode* string_ = nullptr;

  public:
    explicit Parser(std::shared_ptr<Scanner> scanner, std::shared_ptr<Logger> logger);

    ~Parser();

    /**
     * \brief Parses the input provided by the scanner and creates a semantically checked abstract
     * syntax tree. In case of syntactic or semantic errors the function throws a ParseException.
     *
     * \return the checked abstract syntax tree of the input
     */
    std::unique_ptr<ModuleNode> parse();

  private:
    /**
     * \brief Checks the next tokens type
     * \param type required token type
     * \return true if the token type matched
     */
    bool checkToken(const TokenType& type) const;

    /**
     * \brief Checks the next tokens type and consumes it if it has the specified type
     * \param type required token type
     * \return true if the token type matched
     */
    bool checkAndConsumeToken(const TokenType& type) const;

    /**
     * \brief Asserts that the next token has the specified type and consumes the token silently.
     * \param type required token type
     * \throws ParseException if the token type mismatches.
     */
    void requireToken(const TokenType& type) const;

    /**
     * \brief Asserts that the next token has the specified type and returns the token.
     * \param type required token type
     * \return the read token
     * \throws ParseException if the token type mismatches.
     */
    std::unique_ptr<const Token> requireAndGetToken(const TokenType& type) const;

    /**
     * \brief Reads the next token as an identifier.
     * \return Identifier instance with the read position and name
     * \throws ParseException if the token is not an identifier
     */
    Identifier ident() const;

    /**
     * \brief Reads an identifier list.
     * \return The vector of Identifier instances read.
     * \throws ParseException if there is no valid Identifier list
     */
    std::vector<Identifier> ident_list() const;

    // parsing functions
    std::unique_ptr<ModuleNode> module();
    void declarations(BlockNode* block);
    void const_declaration(std::vector<std::unique_ptr<ConstantDeclarationNode>>& list);
    void type_declaration(std::vector<std::unique_ptr<TypeDeclarationNode>>& list);
    void var_declaration(BlockNode* parent);
    void procedure_declaration(std::vector<std::unique_ptr<ProcedureDeclarationNode>>& list);
    void formal_parameters(std::vector<std::unique_ptr<ParameterDeclarationNode>>& list);
    void fp_section(std::vector<std::unique_ptr<ParameterDeclarationNode>>& list);
    std::unique_ptr<ExpressionNode> expression();
    std::unique_ptr<ExpressionNode> simple_expression();
    std::unique_ptr<ExpressionNode> term();
    std::unique_ptr<ExpressionNode> factor();
    TypeNode* type();
    void field_list(MemberLayout<FieldDeclarationNode>& list);
    void actual_parameters(std::vector<std::unique_ptr<ExpressionNode>>& list);
    void statement_sequence(std::vector<std::unique_ptr<StatementNode>>& list);
    std::unique_ptr<StatementNode> statement();
    std::unique_ptr<AssignmentNode> assignment(const Identifier& id);
    std::unique_ptr<ProcedureCallNode> procedure_call(const Identifier& id);
    std::unique_ptr<IfStatementNode> if_statement();
    std::unique_ptr<WhileStatementNode> while_statement();
    std::unique_ptr<AssignableExpressionNode>
    selector(std::unique_ptr<AssignableExpressionNode> parent);

    // expression evaluation
    std::unique_ptr<ExpressionNode>
    evaluateBinaryExpression(std::unique_ptr<ExpressionNode> operand_1,
                             std::unique_ptr<ExpressionNode> operand_2,
                             std::unique_ptr<const Token> op) const;
    std::unique_ptr<ExpressionNode> evaluateUnaryExpression(std::unique_ptr<ExpressionNode> operand,
                                                            std::unique_ptr<const Token> op) const;

    // type checking
    TypeNode*
    typeCheckBinary(ExpressionNode* operand_1, ExpressionNode* operand_2, OperatorType op) const;
    TypeNode* typeCheckUnary(ExpressionNode* operand, OperatorType op) const;

    template <class T>
    void insertDeclaration(std::unique_ptr<T> node, std::vector<std::unique_ptr<T>>& list)
    {
        if (current_scope_->declareIdentifier(node->getName(), node.get())) {
            list.push_back(std::move(node));
        } else {
            throw ParseException(node->getFilePos(), error_id::E001, node->getName());
        }
    }

    // type and scope handling
    Node* resolveLocalId(const Scope* scope, const Identifier& id) const;
    Node* resolveLocalId(const Scope* scope, const std::string& name, const FilePos& pos) const;
    Node* resolveId(const Scope* scope, const Identifier& id) const;
    Node* resolveId(const Scope* scope, const std::string& name, const FilePos& pos) const;
    TypeNode* addType(std::unique_ptr<TypeNode> type);
    TypeNode* findType(TypeNode* type, const FilePos& pos) const;
    TypeNode* selectUnaryOperationType(UnaryOperator op) const;
    TypeNode* selectBinaryOperationType(BinaryOperator op) const;
};
