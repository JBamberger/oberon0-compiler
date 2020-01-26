
#include "X86_64CodeGenerator.h"
#include "BinaryExpressionNode.h"
#include "UnaryExpressionNode.h"
#include "NumberConstantNode.h"
#include "StringConstantNode.h"

X86_64CodeGenerator::X86_64CodeGenerator() : output_(nullptr), nl_("\n"), label_nr_(1) {}

X86_64CodeGenerator::~X86_64CodeGenerator() = default;

void X86_64CodeGenerator::generate(std::unique_ptr<ModuleNode> ast, std::ostream *output) {
    output_ = output;
    ast->visit(this);
    output_ = nullptr;
}

void X86_64CodeGenerator::visit(const ModuleNode *node) {
    *output_ << "%pragma macho64 prefix _" << nl_
             << "        global  main" << nl_
             << "        extern printf                   ; libc printf(string, args)" << nl_
             << nl_
             << "section .data" << nl_
             << "dmsg:" << nl_
             << "        db  'Debug output %d', 0" << nl_;
    for (const auto &c : node->getConstants()) {
        c->visit(this);
    }
    for (const auto &v : node->getVariables()) {
        v->visit(this);
    }
    *output_ << nl_
             << "section .text" << nl_
             << "main:                                   ; Module begin: " << node->getName() << nl_
             << "        push    rbp                     ; save caller stack frame" << nl_
             << "        mov     rsp, rbp                ; move to next stack frame" << nl_
             // here goes the module body

             // << "; ----- debug ----------------------------------------------------------" << nl_
             // << "        mov     rcx, dmsg               ; debug message -> param 1" << nl_
             // << "        pop     rdx                     ; most recent result -> param 2" << nl_
             // << "        call    printf                  ; print the debug value" << nl_
             // << "; ----- debug end ------------------------------------------------------" << nl_

             << "        xor     rax, rax                ; set return value to 0" << nl_
             << "        mov     rsp, rbp                ; reset stack pointer for caller" << nl_
             << "        pop     rbp                     ; reset stack base for caller" << nl_
             << "        ret                             ; Module end: " << node->getName() << nl_
             // here go the modules functions
             << nl_;
}

void X86_64CodeGenerator::visit(const ConstantDeclarationNode *node) {
    const auto label = node->getName() + nextLabel();
    // Padding for the comment following the label. comment Column is 40 but the label is followed by a colon.
    const auto pad_string = std::string(label.size() > 39L ? 0 : 39L - label.size(), ' ');

    const auto type = node->getValue()->getType();
    if (type == "INTEGER") {
        const auto value = dynamic_cast<NumberConstantNode *>(node->getValue().get())->getValue();
        *output_ << label << ":" << pad_string << "; integer constant definition" << nl_
                 << "        dq      " << std::to_string(value) << nl_;
    } else if (type == "STRING") {
        //TODO: escape the string appropriately
        const auto value = dynamic_cast<StringConstantNode *>(node->getValue().get())->getValue();
        *output_ << "        ; string constant definition" << nl_
                 << label << ":" << nl_
                 << "        db      '" << value << "', 0" << nl_;
    } else {
        throw std::runtime_error("Only integer constants are supported.");
    }

    //TODO: save label name of the constant
}

void X86_64CodeGenerator::visit(const VariableDeclarationNode *) {
}


void X86_64CodeGenerator::visit(const ArrayReferenceNode *node) {
}

void X86_64CodeGenerator::visit(const VariableReferenceNode *node) {
}

void X86_64CodeGenerator::visit(const FieldReferenceNode *node) {
}

void X86_64CodeGenerator::visit(const AssignmentNode *node) {
}

void X86_64CodeGenerator::visit(const BinaryExpressionNode *node) {
    // push op 1 to the stack
    node->getOperand1()->visit(this);
    // push op 2 to the stack
    node->getOperand2()->visit(this);

    switch (node->getOperator()) {
        case BinaryOperator::plus:
            *output_ << "        ; addition" << nl_
                     << "        pop     rax" << nl_
                     << "        pop     rbx" << nl_
                     << "        add     rax, rbx" << nl_
                     << "        push    rax" << nl_;
            break;
        case BinaryOperator::minus:
            *output_ << "        ; subtraction" << nl_
                     << "        pop     rax" << nl_
                     << "        pop     rbx" << nl_
                     << "        sub     rax, rbx" << nl_
                     << "        push    rax" << nl_;
            break;
        case BinaryOperator::times:
            *output_ << "        ; multiplication" << nl_
                     << "        pop     rax" << nl_
                     << "        pop     rbx" << nl_
                     << "        imul    rax, rbx                ; perform signed multiplication" << nl_
                     << "        push    rax" << nl_;
            break;
        case BinaryOperator::div:
            *output_ << "        ; signed integer division" << nl_
                     << "        pop     rax" << nl_
                     << "        pop     rbx" << nl_
                     << "        xor     rdx, rdx                ; zero out rdx" << nl_
                     << "        cqo                             ; build rdx:rax by sign extension" << nl_
                     << "        idiv    rbx                     ; perform signed division" << nl_
                     << "        push    rax" << nl_;
            break;
        case BinaryOperator::mod: {
            // The idiv instruction only computes the remainder. The Oberon language requires the mathematical modulus
            // instead of the remainder. The remainder can be negative if the numbers are negative.
            // Let m be the remainder of a / b. The modulus can then be computed as follows:
            // m >= 0          -> modulus = m
            // m < 0 && b <  0 -> modulus = m - b
            // m < 0 && b >= 0 -> modulus = m + b
            const auto jumpLabel = ".MOD_" + nextLabel();
            *output_ << "        ; modulus" << nl_
                     << "        pop     rax" << nl_
                     << "        pop     rbx" << nl_
                     << "        xor     rdx, rdx                ; zero out rdx" << nl_
                     << "        cqo                             ; build rdx:rax by sign extension" << nl_
                     << "        idiv    rbx                     ; perform signed division" << nl_
                     << "        mov     rax, rdx                ; set return value" << nl_
                     << "        test    rdx, rdx                ; sets sign bit if negative" << nl_
                     << "        jns     " << jumpLabel << "                ; finished if non-zero" << nl_
                     << "        sub     rax, rbx                ; rax -= rbx // first neg. branch" << nl_
                     << "        add     rdx, rbx                ; rdx += rbx // second neg. branch" << nl_
                     << "        test    rbx, rbx                ; check if rbx is negative" << nl_
                     << "        cmovns  rax, rdx                ; use rdx if rbx is negative" << nl_
                     << jumpLabel << ":" << nl_
                     << "        push    rdx" << nl_;
        }
            break;
        case BinaryOperator::logical_or:
            *output_ << "        ; logical or" << nl_
                     << "        pop     rbx" << nl_
                     << "        pop     rcx" << nl_
                     << "        xor     eax, eax" << nl_
                     << "        or      rbx, rcx" << nl_
                     << "        setne   al" << nl_
                     << "        push    rax" << nl_;
        case BinaryOperator::logical_and:
            *output_ << "        ; logical and" << nl_
                     << "        pop     rbx" << nl_
                     << "        pop     rcx" << nl_
                     << "        test    rbx, rbx                ; make first operand to real bool" << nl_
                     << "        setne   dl                      ; rdx is now real bool" << nl_
                     << "        xor     eax, eax                ; zero out rdx" << nl_
                     << "        test    rcx, rcx                ; make second operand real bool" << nl_
                     << "        setne   al                      ; rax is now real bool" << nl_
                     << "        and     rax, rdx                ; a && b" << nl_
                     << "        push    rax" << nl_;
        default:
            std::string op;
            std::string name;
            switch (node->getOperator()) {
                case BinaryOperator::eq:
                    op = "sete    ";
                    name = "equals";
                    break;
                case BinaryOperator::neq:
                    op = "setne   ";
                    name = "not equals";
                    break;
                case BinaryOperator::lt:
                    op = "setl    ";
                    name = "less than";
                    break;
                case BinaryOperator::leq:
                    op = "setle   ";
                    name = "less than or equals";
                    break;
                case BinaryOperator::gt:
                    op = "setg    ";
                    name = "greater than";
                    break;
                case BinaryOperator::geq:
                    op = "setge   ";
                    name = "greater than or equals";
                    break;
                default:
                    throw std::runtime_error("Unsupported operation!");
            }
            *output_ << "        ; " << name << nl_
                     << "        pop     rbx" << nl_
                     << "        pop     rcx" << nl_
                     << "        xor     eax, eax                ; zero out rdx" << nl_
                     << "        comp    rbx, rcx                ; compare a and b" << nl_
                     << "        " << op << "al                      ; set rax to result" << nl_
                     << "        push    rax" << nl_;

    }
}

void X86_64CodeGenerator::visit(const UnaryExpressionNode *node) {
    node->getOperand()->visit(this); // push operand

    switch (node->getOperator()) {
        case UnaryOperator::plus:
            // Nothing to do. The number remains as is and the stack is not modified.
            break;
        case UnaryOperator::minus:
            *output_ << "        ; negation" << nl_
                     << "        pop     rax" << nl_
                     << "        neg     rax" << nl_
                     << "        push    rax" << nl_;
            break;
        case UnaryOperator::inverse:
            // initial version:
            // << "        pop     rbx" << nl_
            // << "        xor     rax, rax                ; zero output register" << nl_
            // << "        test    rbx, rbx                ; prep zero check" << nl_
            // << "        sete    al                      ; is false? -> set rax" << nl_
            // << "        push    rax" << nl_;

            // This version requires only one register. Using eax with move is better than rax because eax yields a
            // shorter op code and the upper bits are zeroed anyway. (In this case irrelevant as the performance is
            // bad anyway :P
            *output_ << "        ; bitwise not" << nl_
                     << "        pop     rax" << nl_
                     << "        cmp     rax, 0                  ; perp zero check" << nl_
                     << "        sete    al                      ; is zero / false?" << nl_
                     << "        movzx   eax, al                 ; zero extend to rax/eax" << nl_
                     << "        push    rax" << nl_;
            break;
    }

}

void X86_64CodeGenerator::visit(const ArrayTypeNode *) {
}

void X86_64CodeGenerator::visit(const TypeDeclarationNode *) {
}


void X86_64CodeGenerator::visit(const ProcedureDeclarationNode *) {
}

void X86_64CodeGenerator::visit(const FieldDeclarationNode *) {
}

void X86_64CodeGenerator::visit(const IfStatementNode *) {
}

void X86_64CodeGenerator::visit(const NumberConstantNode *) {
}

void X86_64CodeGenerator::visit(const StringConstantNode *) {
    throw std::runtime_error("String constants are not supported.");
}

void X86_64CodeGenerator::visit(const ParameterDeclarationNode *) {
}

void X86_64CodeGenerator::visit(const ProcedureCallNode *) {
}

void X86_64CodeGenerator::visit(const RecordTypeNode *) {
}

void X86_64CodeGenerator::visit(const TypedIdentifierNode *) {
}

void X86_64CodeGenerator::visit(const BasicTypeNode *) {
}

void X86_64CodeGenerator::visit(const WhileStatementNode *) {
}

std::string X86_64CodeGenerator::nextLabel() {
    return std::to_string(label_nr_++);
}
