#include "PrintVisitor.h"

#include "ActualParameterNode.h"
#include "ArrayReferenceNode.h"
#include "ArrayTypeNode.h"
#include "AssignmentNode.h"
#include "BasicTypeNode.h"
#include "BinaryExpressionNode.h"
#include "FieldReferenceNode.h"
#include "IfStatementNode.h"
#include "ModuleNode.h"
#include "NumberConstantNode.h"
#include "ProcedureCallNode.h"
#include "RecordTypeNode.h"
#include "StringConstantNode.h"
#include "UnaryExpressionNode.h"
#include "WhileStatementNode.h"
#include "ProcedureDeclarationNode.h"

PrintVisitor::PrintVisitor(std::ostream& out) : out_(out), indent_(0) {}

PrintVisitor::~PrintVisitor() {}

std::ostream& PrintVisitor::line()
{
    for (int i = 0; i < indent_; ++i) {
        out_ << "|   ";
    }
    return out_;
}

void PrintVisitor::inc() { indent_++; }

void PrintVisitor::dec() { indent_--; }

void PrintVisitor::visit(const ActualParameterNode* node)
{
    line() << "ActualParameter\n";
    inc();
    NodeVisitor::visit(node);
    dec();
}

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
    line() << "BinaryExpression" << node->getOperator() << "\n";
    inc();
    node->getOperand1()->visit(this);
    node->getOperand2()->visit(this);
    dec();
}

void PrintVisitor::visit(const FieldReferenceNode* node)
{
    line() << "FieldReference " << node->getName() << "\n";
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

void PrintVisitor::visit(const FormalParameterList* node)
{
    line() << "FormalParameterList\n";
    inc();
    NodeVisitor::visit(node);
    dec();
}

void PrintVisitor::visit(const DeclarationsNode* node)
{
    NodeVisitor::visit(node);
}

void PrintVisitor::visit(const FieldNode* node)
{
    line() << "Field " << node->getName() << "\n";
    inc();
    NodeVisitor::visit(node);
    dec();
}

void PrintVisitor::visit(const IdentifierListNode* node)
{
    line() << "IdentifierList\n";
    inc();
    for (const auto& name : node->getNames()) {
        line() << name << "\n";
    }
    dec();
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
    NodeVisitor::visit(node);
    dec();
}

void PrintVisitor::visit(const NumberConstantNode* node)
{
    line() << "Number " << node->getValue() << "\n";
}

void PrintVisitor::visit(const ParameterNode* node)
{
    line() << "Parameter " << node->getName() << (node->isIsReference() ? "Ref\n" : "\n");
    inc();
    NodeVisitor::visit(node);
    dec();
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
    NodeVisitor::visit(node);
    dec();
}

void PrintVisitor::visit(const RecordTypeNode* node)
{
    line() << "RecordType\n";
    inc();
    NodeVisitor::visit(node);
    dec();
}

void PrintVisitor::visit(const StatementSequenceNode* node)
{
    line() << "StatementSequence\n";
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
    line() << "TypeDeclaration " << node->getName() << "\n";
    inc();
    node->getType()->visit(this);
    dec();
}

void PrintVisitor::visit(const FieldListNode* node)
{
    line() << "FieldList\n";
    inc();
    NodeVisitor::visit(node);
    dec();
}

void PrintVisitor::visit(const ParameterListNode* node)
{
    line() << "ParameterList\n";
    inc();
    NodeVisitor::visit(node);
    dec();
}

void PrintVisitor::visit(const VariableListNode* node)
{
    line() << "VariableList\n";
    inc();
    NodeVisitor::visit(node);
    dec();
}

void PrintVisitor::visit(const TypedIdentifierNode* node)
{
    line() << "TypedIdentifier " << node->getName() << "\n";
    inc();
    node->getType()->visit(this);
    dec();
}

void PrintVisitor::visit(const TypeReferenceNode* node)
{
    line() << "TypeReference\n";
    inc();
    node->getType()->visit(this);
    dec();
}

void PrintVisitor::visit(const UnaryExpressionNode* node)
{
    line() << "UnaryExpression" << node->getOperator() << "\n";
    inc();
    node->getOperand()->visit(this);
    dec();
}

void PrintVisitor::visit(const VariableNode* node)
{
    line() << "VariableDeclaration " << node->getName() << "\n";
    inc();
    NodeVisitor::visit(node);
    dec();
}

void PrintVisitor::visit(const VariableReferenceNode* node)
{
    line() << "VariableReference " << node->getName() << "\n";
    inc();
    if (node->getSelector()) {
        node->getSelector()->visit(this);
    }
    dec();
}

void PrintVisitor::visit(const WhileStatementNode* node)
{
    line() << "While\n";
    inc();
    NodeVisitor::visit(node);
    dec();
}
