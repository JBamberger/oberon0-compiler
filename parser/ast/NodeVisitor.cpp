#include "NodeVisitor.h"
#include "ActualParameterNode.h"
#include "ArrayReferenceNode.h"
#include "ArrayTypeNode.h"
#include "AssignmentNode.h"
#include "BasicTypeNode.h"
#include "BinaryExpressionNode.h"
#include "ConstantDeclarationNode.h"
#include "FieldReferenceNode.h"
#include "IfStatementNode.h"
#include "ModuleNode.h"
#include "NumberConstantNode.h"
#include "ProcedureCallNode.h"
#include "ProcedureDeclarationNode.h"
#include "RecordTypeNode.h"
#include "StringConstantNode.h"
#include "TypeDeclarationNode.h"
#include "TypedIdentifierListNode.h"
#include "UnaryExpressionNode.h"
#include "VariableNode.h"
#include "WhileStatementNode.h"

NodeVisitor::~NodeVisitor() = default;

void NodeVisitor::visit(const ActualParameterNode* node)
{
    node->getParam()->visit(this);
    if (node->getNext() != nullptr) {
        node->getNext()->visit(this);
    }
}

void NodeVisitor::visit(const ArrayReferenceNode* node) { node->getIndex()->visit(this); }

void NodeVisitor::visit(const ArrayTypeNode* node)
{
    node->getValue()->visit(this);
    node->getType()->visit(this);
}

void NodeVisitor::visit(const AssignmentNode* node)
{
    node->getAssignee()->visit(this);
    node->getValue()->visit(this);
}

void NodeVisitor::visit(const BasicTypeNode* node) {}

void NodeVisitor::visit(const BinaryExpressionNode* node)
{
    node->getOperand1()->visit(this);
    node->getOperand2()->visit(this);
}

void NodeVisitor::visit(const ConstantDeclarationNode* node) { node->getValue()->visit(this); }

void NodeVisitor::visit(const FieldReferenceNode* node)
{
    if (node->getNext()) {
        node->getNext()->visit(this);
    }
}

void NodeVisitor::visit(const FormalParameterList* node)
{
    for (const auto& decl : node->getList()) {
        decl->visit(this);
    }
}

void NodeVisitor::visit(const DeclarationsNode* node)
{

    for (const auto& decl : *node->getConstants()) {
        decl->visit(this);
    }
    for (const auto& decl : *node->getVariables()) {
        decl->visit(this);
    }
    for (const auto& decl : *node->getTypes()) {
        decl->visit(this);
    }
    for (const auto& decl : *node->getProcedures()) {
        decl->visit(this);
    }
}

void NodeVisitor::visit(const FieldNode* node) { node->getType()->visit(this); }

void NodeVisitor::visit(const IdentifierListNode* node) {}

void NodeVisitor::visit(const IfStatementNode* node)
{
    node->getCondition()->visit(this);
    node->getThenPart()->visit(this);
    if (node->getElsePart() != nullptr) {
        node->getElsePart()->visit(this);
    }
}

void NodeVisitor::visit(const ModuleNode* node)
{
    node->getDeclarations()->visit(this);
    node->getStatements()->visit(this);
}

void NodeVisitor::visit(const NumberConstantNode* node) {}

void NodeVisitor::visit(const ParameterNode* node) { node->getType()->visit(this); }

void NodeVisitor::visit(const ProcedureCallNode* node)
{
    if (node->getParameters() != nullptr) {
        node->getParameters()->visit(this);
    }
}

void NodeVisitor::visit(const ProcedureDeclarationNode* node)
{
    if (node->getParams() != nullptr) {
        node->getParams()->visit(this);
    }
    node->getDeclarations()->visit(this);
    node->getStatements()->visit(this);
}

void NodeVisitor::visit(const RecordTypeNode* node)
{
    for (const auto& f : node->getMembers()) {
        f->visit(this);
    }
    for (const auto& t : node->getTypes()) {
        t->visit(this);
    }
}

void NodeVisitor::visit(const StatementSequenceNode* node)
{
    for (const auto& s : node->getBody()) {
        s->visit(this);
    }
}

void NodeVisitor::visit(const StringConstantNode* node) {}

void NodeVisitor::visit(const TypeDeclarationNode* node) { node->getType()->visit(this); }

void NodeVisitor::visit(const FieldListNode* node)
{
    node->getType()->visit(this);
    for (const auto& f : *node->getPairs()) {
        f->visit(this);
    }
}

void NodeVisitor::visit(const ParameterListNode* node)
{
    node->getType()->visit(this);
    for (const auto& f : *node->getPairs()) {
        f->visit(this);
    }
}

void NodeVisitor::visit(const VariableListNode* node)
{
    node->getType()->visit(this);
    for (const auto& f : *node->getPairs()) {
        f->visit(this);
    }
}

void NodeVisitor::visit(const TypedIdentifierNode* node) { node->getType()->visit(this); }

void NodeVisitor::visit(const TypeReferenceNode* node) { node->getType()->visit(this); }

void NodeVisitor::visit(const UnaryExpressionNode* node) { node->getOperand()->visit(this); }

void NodeVisitor::visit(const VariableNode* node) { node->getType()->visit(this); }

void NodeVisitor::visit(const VariableReferenceNode* node)
{
    if (node->getSelector() != nullptr) {
        node->getSelector()->visit(this);
    }
}

void NodeVisitor::visit(const WhileStatementNode* node)
{
    node->getCondition()->visit(this);
    node->getBody()->visit(this);
}