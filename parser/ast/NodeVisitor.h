#pragma once

class BlockNode;
class FieldReferenceNode;
class SelectorNode;
class FieldDeclarationNode;
class ParameterDeclarationNode;
class ConstantDeclarationNode;
class IfStatementNode;
class NumberConstantNode;
class ProcedureBodyNode;
class ProcedureCallNode;
class ProcedureDeclarationNode;
class ProcedureHeadingNode;
class RecordTypeNode;
class StringConstantNode;
class TypeDeclarationNode;
class TypedIdentifierNode;
class UnaryExpressionNode;
class VariableDeclarationNode;
class VariableReferenceNode;
class WhileStatementNode;
class BinaryExpressionNode;
class ChainedReferenceNode;
class BasicTypeNode;
class AssignmentNode;
class ArrayTypeNode;
class ArrayReferenceNode;
class ActualParameterNode;
class ModuleNode;

class NodeVisitor {

  public:
    virtual ~NodeVisitor() = 0;

    virtual void visit(const ActualParameterNode* node) = 0;
    virtual void visit(const ArrayReferenceNode* node) = 0;
    virtual void visit(const ArrayTypeNode* node) = 0;
    virtual void visit(const AssignmentNode* node) = 0;

    virtual void visit(const BinaryExpressionNode* node) = 0;
    virtual void visit(const UnaryExpressionNode* node) = 0;

    virtual void visit(const ConstantDeclarationNode* node) = 0;
    virtual void visit(const TypeDeclarationNode* node) = 0;
    virtual void visit(const VariableDeclarationNode* node) = 0;
    virtual void visit(const ProcedureDeclarationNode* node) = 0;

    virtual void visit(const FieldReferenceNode* node) = 0;
    virtual void visit(const FieldDeclarationNode* node) = 0;
    virtual void visit(const IfStatementNode* node) = 0;
    virtual void visit(const ModuleNode* node) = 0;
    virtual void visit(const NumberConstantNode* node) = 0;
    virtual void visit(const ParameterDeclarationNode* node) = 0;
    virtual void visit(const ProcedureCallNode* node) = 0;
    virtual void visit(const RecordTypeNode* node) = 0;
    virtual void visit(const StringConstantNode* node) = 0;
    virtual void visit(const TypedIdentifierNode* node) = 0;
    virtual void visit(const BasicTypeNode* node) = 0;
    virtual void visit(const VariableReferenceNode* node) = 0;
    virtual void visit(const WhileStatementNode* node) = 0;
};
