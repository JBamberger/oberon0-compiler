#include "PrintVisitor.h"

#include "ast/ArrayReferenceNode.h"
#include "ast/ArrayTypeNode.h"
#include "ast/AssignmentNode.h"
#include "ast/BasicTypeNode.h"
#include "ast/BinaryExpressionNode.h"
#include "ast/BooleanConstantNode.h"
#include "ast/FieldReferenceNode.h"
#include "ast/IfStatementNode.h"
#include "ast/ModuleNode.h"
#include "ast/NumberConstantNode.h"
#include "ast/ParameterReferenceNode.h"
#include "ast/ProcedureCallNode.h"
#include "ast/ProcedureDeclarationNode.h"
#include "ast/RecordTypeNode.h"
#include "ast/StringConstantNode.h"
#include "ast/UnaryExpressionNode.h"
#include "ast/WhileStatementNode.h"

PrintVisitor::PrintVisitor(std::ostream& out) : out_(out), indent_(0) {}

PrintVisitor::~PrintVisitor() {}

std::ostream& PrintVisitor::line()
{
    for (auto i = 0; i < indent_; ++i) {
        out_ << "|   ";
    }
    return out_;
}

void PrintVisitor::inc() { indent_++; }

void PrintVisitor::dec() { indent_--; }

void PrintVisitor::visit(const ArrayReferenceNode* node)
{
    line() << "ArrayReference\n";
    inc();
    NodeVisitor::visit(node);
    dec();
}

void PrintVisitor::visit(const ArrayTypeNode* node)
{
    line() << "ArrayType\n";
    inc();
    line() << node->getSize() << "\n";
    line() << node->getType() << "\n";
    NodeVisitor::visit(node);
    dec();
}

void PrintVisitor::visit(const AssignmentNode* node)
{
    line() << "AssignmentNode\n";
    inc();
    NodeVisitor::visit(node);
    dec();
}

void PrintVisitor::visit(const BasicTypeNode* node)
{
    line() << "BasicType " << node->getName() << "\n";
}

void PrintVisitor::visit(const BinaryExpressionNode* node)
{
    line() << "BinaryExpression " << node->getOperator() << "\n";
    inc();
    node->getOperand1()->visit(this);
    node->getOperand2()->visit(this);
    dec();
}

void PrintVisitor::visit(const FieldReferenceNode* node)
{
    line() << "FieldReference " << node->getFieldName() << "\n";
    inc();
    NodeVisitor::visit(node);
    dec();
}

void PrintVisitor::visit(const ConstantDeclarationNode* node)
{
    line() << "ConstantDeclaration " << node->getName() << "\n";
    inc();
    NodeVisitor::visit(node);
    dec();
}

void PrintVisitor::visit(const FieldDeclarationNode* node)
{
    line() << "Field " << node->getName() << " : " << node->getType() << "\n";
}

void PrintVisitor::visit(const IfStatementNode* node)
{
    line() << "If\n";
    inc();
    NodeVisitor::visit(node);
    dec();
}

void PrintVisitor::visit(const ModuleNode* node)
{
    line() << "Module " << node->getName() << "\n";
    inc();
    printBlock(node);
    dec();

    out_ << "\n" << *node->getScope() << std::endl;
}

void PrintVisitor::visit(const NumberConstantNode* node)
{
    line() << "Number " << node->getValue() << "\n";
}

void PrintVisitor::visit(const ParameterDeclarationNode* node)
{
    line() << "Parameter " << (node->isIsReference() ? "*" : "") << node->getName() << "\n";
}

void PrintVisitor::visit(const ProcedureCallNode* node)
{
    line() << "ProcedureCall " << node->getName() << "\n";
    inc();
    NodeVisitor::visit(node);
    dec();
}

void PrintVisitor::visit(const ProcedureDeclarationNode* node)
{
    line() << "ProcedureDeclaration " << node->getName() << "\n";
    inc();

    if (!node->getParams()->empty()) {
        line() << "Parameters:\n";
        inc();
        for (const auto& s : *node->getParams()) {
            s->visit(this);
        }
        dec();
    }
    printBlock(node);
    dec();
}

void PrintVisitor::visit(const RecordTypeNode* node)
{
    line() << "RecordType\n";
    inc();
    NodeVisitor::visit(node);
    dec();
}

void PrintVisitor::visit(const StringConstantNode* node)
{
    line() << "String(" << node->getValue() << ")\n";
}

void PrintVisitor::visit(const TypeDeclarationNode* node)
{
    line() << "TypeDeclaration " << node->getName() << " : " << node->getType() << "\n";
}

void PrintVisitor::visit(const TypedIdentifierNode* node)
{
    line() << "TypedIdentifier " << node->getName() << " : " << node->getType() << "\n";
}

void PrintVisitor::visit(const UnaryExpressionNode* node)
{
    line() << "UnaryExpression" << node->getOperator() << "\n";
    inc();
    node->getOperand()->visit(this);
    dec();
}

void PrintVisitor::visit(const VariableDeclarationNode* node)
{
    line() << "VariableDeclaration " << node->getName() << " : " << node->getType() << "\n";
}

void PrintVisitor::visit(const VariableReferenceNode* node)
{
    line() << "VariableReference " << node->getName() << "\n";
}

void PrintVisitor::visit(const ParameterReferenceNode* node)
{
    line() << "ParameterReference " << node->getName() << "\n";
}

void PrintVisitor::visit(const WhileStatementNode* node)
{
    line() << "While\n";
    inc();
    NodeVisitor::visit(node);
    dec();
}

void PrintVisitor::visit(const BooleanConstantNode* node)
{
    line() << "BooleanConstant " << node->getValue() << "\n";
}

void PrintVisitor::printBlock(const BlockNode* node)
{
    if (!node->getConstants()->empty()) {
        line() << "Constants:\n";
        inc();
        for (const auto& decl : *node->getConstants()) {
            decl->visit(this);
        }
        dec();
    }
    if (!node->getTypes()->empty()) {
        line() << "Types:\n";
        inc();
        for (const auto& decl : *node->getTypes()) {
            decl->visit(this);
        }
        dec();
    }
    if (!node->getVariables()->empty()) {
        line() << "Variables:\n";
        inc();
        for (const auto& decl : *node->getVariables()) {
            decl->visit(this);
        }
        dec();
    }
    if (!node->getProcedures()->empty()) {
        line() << "Procedures:\n";
        inc();
        for (const auto& decl : *node->getProcedures()) {
            decl->visit(this);
        }
        dec();
    }
    if (!node->getStatements()->empty()) {
        line() << "Statements:\n";
        inc();
        for (const auto& s : *node->getStatements()) {
            s->visit(this);
        }
        dec();
    }
}
