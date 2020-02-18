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

#include "PrintVisitor.h"

#include "ArrayReferenceNode.h"
#include "ArrayTypeNode.h"
#include "AssignmentNode.h"
#include "BasicTypeNode.h"
#include "BinaryExpressionNode.h"
#include "BooleanConstantNode.h"
#include "FieldReferenceNode.h"
#include "IfStatementNode.h"
#include "ModuleNode.h"
#include "NumberConstantNode.h"
#include "ParameterReferenceNode.h"
#include "ProcedureCallNode.h"
#include "ProcedureDeclarationNode.h"
#include "RecordTypeNode.h"
#include "StringConstantNode.h"
#include "UnaryExpressionNode.h"
#include "WhileStatementNode.h"

PrintVisitor::PrintVisitor(std::ostream& out) : out_(out), indent_(0)
{
}

PrintVisitor::~PrintVisitor() = default;

std::ostream& PrintVisitor::line() const
{
    for (auto i = 0; i < indent_; ++i) {
        out_ << "|   ";
    }
    return out_;
}

void PrintVisitor::inc()
{
    indent_++;
}

void PrintVisitor::dec()
{
    indent_--;
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

    if (!node->getParams().empty()) {
        line() << "Parameters:\n";
        inc();
        for (const auto& s : node->getParams()) {
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
    if (!node->getConstants().empty()) {
        line() << "Constants:\n";
        inc();
        for (const auto& decl : node->getConstants()) {
            decl->visit(this);
        }
        dec();
    }
    if (!node->getTypes().empty()) {
        line() << "Types:\n";
        inc();
        for (const auto& decl : node->getTypes()) {
            decl->visit(this);
        }
        dec();
    }
    if (node->getVariables().getSize() > 0) {
        line() << "Variables:\n";
        inc();
        node->getVariables().visitMembers([this](const auto& m) { m.member->visit(this); });
        dec();
    }
    if (!node->getProcedures().empty()) {
        line() << "Procedures:\n";
        inc();
        for (const auto& decl : node->getProcedures()) {
            decl->visit(this);
        }
        dec();
    }
    if (!node->getStatements().empty()) {
        line() << "Statements:\n";
        inc();
        for (const auto& s : node->getStatements()) {
            s->visit(this);
        }
        dec();
    }
}
