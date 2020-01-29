#pragma once

#include "NodeVisitor.h"
class ThrowingNodeVisitor : public NodeVisitor {
  public:
    void visit(const ArrayReferenceNode*) override
    {
        throw std::runtime_error("Unsupported operation.");
    }
    void visit(const ArrayTypeNode*) override
    {
        throw std::runtime_error("Unsupported operation.");
    }
    void visit(const AssignmentNode*) override
    {
        throw std::runtime_error("Unsupported operation.");
    }
    void visit(const BinaryExpressionNode*) override
    {
        throw std::runtime_error("Unsupported operation.");
    }
    void visit(const UnaryExpressionNode*) override
    {
        throw std::runtime_error("Unsupported operation.");
    }
    void visit(const ConstantDeclarationNode*) override
    {
        throw std::runtime_error("Unsupported operation.");
    }
    void visit(const TypeDeclarationNode*) override
    {
        throw std::runtime_error("Unsupported operation.");
    }
    void visit(const VariableDeclarationNode*) override
    {
        throw std::runtime_error("Unsupported operation.");
    }
    void visit(const ProcedureDeclarationNode*) override
    {
        throw std::runtime_error("Unsupported operation.");
    }
    void visit(const FieldReferenceNode*) override
    {
        throw std::runtime_error("Unsupported operation.");
    }
    void visit(const FieldDeclarationNode*) override
    {
        throw std::runtime_error("Unsupported operation.");
    }
    void visit(const IfStatementNode*) override
    {
        throw std::runtime_error("Unsupported operation.");
    }
    void visit(const ModuleNode*) override
    {
        throw std::runtime_error("Unsupported operation.");
    }
    void visit(const NumberConstantNode*) override
    {
        throw std::runtime_error("Unsupported operation.");
    }
    void visit(const StringConstantNode*) override
    {
        throw std::runtime_error("Unsupported operation.");
    }
    void visit(const ParameterDeclarationNode*) override
    {
        throw std::runtime_error("Unsupported operation.");
    }
    void visit(const ProcedureCallNode*) override
    {
        throw std::runtime_error("Unsupported operation.");
    }
    void visit(const RecordTypeNode*) override
    {
        throw std::runtime_error("Unsupported operation.");
    }
    void visit(const TypedIdentifierNode*) override
    {
        throw std::runtime_error("Unsupported operation.");
    }
    void visit(const BasicTypeNode*) override
    {
        throw std::runtime_error("Unsupported operation.");
    }
    void visit(const VariableReferenceNode*) override
    {
        throw std::runtime_error("Unsupported operation.");
    }
    void visit(const WhileStatementNode*) override
    {
        throw std::runtime_error("Unsupported operation.");
    }
};