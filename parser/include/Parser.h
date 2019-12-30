#pragma once

#include "ArrayTypeNode.h"
#include "AssignmentNode.h"
#include "ExpressionNode.h"
#include "Identifier.h"
#include "IfStatementNode.h"
#include "ModuleNode.h"
#include "Node.h"
#include "ParserErrors.h"
#include "ProcedureCallNode.h"
#include "ProcedureDeclarationNode.h"
#include "RecordTypeNode.h"
#include "Scanner.h"
#include "TypeNode.h"
#include "VariableDeclarationNode.h"
#include "WhileStatementNode.h"

enum class OperatorType : char;
enum class BinaryOperator : char;

class Parser {

    Scanner* scanner_;
    Logger* logger_;

    std::unordered_map<std::string, std::unique_ptr<TypeNode>> types_;
    std::shared_ptr<Scope> current_scope_;

  public:
    explicit Parser(Scanner* scanner, Logger* logger);
    ~Parser();

    /**
     * \brief Parses the input provided by the scanner and creates a semantically checked abstract
     * syntax tree. In case of syntactic or semantic errors the function throws a ParseException.
     *
     * \return the checked abstract syntax tree of the input
     */
    std::unique_ptr<Node> parse();

  private:
    // parsing utility functions
    bool checkToken(const TokenType& type) const;
    bool checkAndConsumeToken(const TokenType& type) const;
    void requireToken(const TokenType& type) const;
    std::unique_ptr<const Token> requireAndGetToken(const TokenType& type) const;

    Identifier ident() const;
    std::vector<Identifier> ident_list() const;

    // parsing functions
    std::unique_ptr<ModuleNode> module();
    void declarations(BlockNode* block);
    void const_declaration(std::vector<std::unique_ptr<ConstantDeclarationNode>>& list);
    void type_declaration(std::vector<std::unique_ptr<TypeDeclarationNode>>& list);
    void var_declaration(std::vector<std::unique_ptr<VariableDeclarationNode>>& list);
    void procedure_declaration(std::vector<std::unique_ptr<ProcedureDeclarationNode>>& list);
    void formal_parameters(std::vector<std::unique_ptr<ParameterDeclarationNode>>& list);
    void fp_section(std::vector<std::unique_ptr<ParameterDeclarationNode>>& list);
    std::unique_ptr<ExpressionNode> expression();
    std::unique_ptr<ExpressionNode> simple_expression();
    std::unique_ptr<ExpressionNode> term();
    std::unique_ptr<ExpressionNode> factor();
    std::string type();
    void field_list(std::vector<std::unique_ptr<FieldDeclarationNode>>& list);
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
    std::string
    typeCheckBinary(ExpressionNode* operand_1, ExpressionNode* operand_2, OperatorType op) const;
    std::string typeCheckUnary(ExpressionNode* operand, OperatorType op) const;

    template <class T>
    void insertDeclaration(std::unique_ptr<T> node, std::vector<std::unique_ptr<T>>& list)
    {
        if (current_scope_->declareIdentifier(node->getName(), node.get())) {
            list.push_back(std::move(node));
        } else {
            throw ParseException(node->getFilePos(), getErrMsg(error_id::E001, node->getName()));
        }
    }

    // type and scope handling
    Node* resolveLocalId(const Scope* scope, const Identifier& id) const;
    Node* resolveLocalId(const Scope* scope, const std::string& name, const FilePos& pos) const;
    Node* resolveId(const Scope* scope, const Identifier& id) const;
    Node* resolveId(const Scope* scope, const std::string& name, const FilePos& pos) const;
    std::string addType(std::unique_ptr<TypeNode> type);
    TypeNode* findType(const std::string& name, const FilePos& pos) const;
};
