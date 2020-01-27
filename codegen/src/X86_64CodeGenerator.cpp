
#include <iomanip>
#include "X86_64CodeGenerator.h"
#include "BinaryExpressionNode.h"
#include "UnaryExpressionNode.h"
#include "NumberConstantNode.h"
#include "StringConstantNode.h"
#include "AssignmentNode.h"
#include "FieldReferenceNode.h"
#include "ArrayReferenceNode.h"

X86_64CodeGenerator::X86_64CodeGenerator() : output_(nullptr), nl_("\n"), label_nr_(1) {}

X86_64CodeGenerator::~X86_64CodeGenerator() = default;

void X86_64CodeGenerator::generate(std::unique_ptr<ModuleNode> ast, std::ostream *output) {
    output_ = output;
    ast->visit(this);
    output_ = nullptr;
}

void X86_64CodeGenerator::defineConstants(const BlockNode *node) {
    // define local constants
    for (const auto &c : node->getConstants()) {
        c->visit(this);
    }
    // recurse subprocedures
    for (const auto &proc : node->getProcedures()) {
        defineConstants(proc.get());
    }
}


void X86_64CodeGenerator::defineVariables(const BlockNode *node) {
    size_t offset = 0;
    for (const auto &v : node->getVariables()) {
        const auto type = v->getType();

    }
}

void X86_64CodeGenerator::visit(const ModuleNode *node) {
    root_scope = node->getScope();
    current_scope = root_scope;


    *output_ << "%pragma macho64 prefix _" << nl_
             << "        global  main" << nl_
             << "        extern printf                   ; libc printf(string, args)" << nl_
             << nl_
             << "section .data" << nl_
             << "dmsg:" << nl_
             << "        db  'Debug output %d', 0" << nl_;
    defineConstants(node);
    for (const auto &v : node->getVariables()) {
        v->visit(this);
    }
    *output_ << nl_
             << "section .text" << nl_
             << "main:                                   ; Module begin: " << node->getName() << nl_
             << "        push    rbp                     ; save caller stack frame" << nl_
             << "        mov     rsp, rbp                ; move to next stack frame" << nl_;
    // here goes the module body

    for (const auto &s : node->getStatements()) {
        s->visit(this);
    }

    // << "; ----- debug ----------------------------------------------------------" << nl_
    // << "        mov     rcx, dmsg               ; debug message -> param 1" << nl_
    // << "        pop     rdx                     ; most recent result -> param 2" << nl_
    // << "        call    printf                  ; print the debug value" << nl_
    // << "; ----- debug end ------------------------------------------------------" << nl_

    *output_ << "        xor     rax, rax                ; set return value to 0" << nl_
             << "        mov     rsp, rbp                ; reset stack pointer for caller" << nl_
             << "        pop     rbp                     ; reset stack base for caller" << nl_
             << "        ret                             ; Module end: " << node->getName() << nl_
             // here go the modules functions
             << nl_;


    current_scope = nullptr;
    root_scope = nullptr;
}

void X86_64CodeGenerator::visit(const ConstantDeclarationNode *node) {
    const auto label = node->getName() + nextLabel();
    const auto type = node->getValue()->getType();
    if (type == "INTEGER") {
        // INTEGER constants are used as immediate value in the instructions
//        const auto value = dynamic_cast<NumberConstantNode *>(node->getValue().get())->getValue();
//        *output_ << padRight(label + ":", COMMENT_COL) << "; integer constant definition" << nl_
//                 << "        dq      " << std::to_string(value) << nl_;
    } else if (type == "STRING") {
        //TODO: escape the string appropriately
        const auto value = dynamic_cast<StringConstantNode *>(node->getValue().get())->getValue();
        *output_ << padRight(label + ":", COMMENT_COL) << "; string constant definition" << nl_
                 << "        db      '" << value << "', 0" << nl_;
    } else {
        throw std::runtime_error("Only integer constants are supported.");
    }

    //TODO: save label name of the constant
}

void X86_64CodeGenerator::visit(const VariableDeclarationNode *node) {
    // TODO: use layout table:

    *output_ << "        ; declare local variable " << node->getName() << nl_
             << "        sub     rbp, size               ; alloc on stack" << nl_
             << "        mov     [rbp, - 8], 0           ; zero out locals";
}

void X86_64CodeGenerator::visit(const ArrayReferenceNode *node) {
    // rbx = array location
    // rax = value
    // mov qword [rbx + index * typeSize], rax
    node->getArrayRef()->visit(this);
    *output_ << "        ; Array value reference" << nl_
             << "        pop     rax                     ; array base address" << nl_
             << "        lea     rax, [rax - offset * typeSize]" << nl_ // TODO: offset, typeSize
             << "        push    rax" << nl_;
}

void X86_64CodeGenerator::visit(const VariableReferenceNode *node) {
    // rbp - variable offset
    *output_ << "        ; Variable reference " << node->getVariable()->getName() << nl_
             << "        pop     rax" << nl_
             << "        lea     rax, [rax - offset]" << nl_ // TODO: offset
             << "        push    rax" << nl_;
}

void X86_64CodeGenerator::visit(const FieldReferenceNode *node) {
    // rbp - record offset - field offset
    node->getRecordRef()->visit(this); // compute record location
    *output_ << "        ; Record field reference " << node->getFieldName() << nl_
             << "        pop     rax" << nl_
             << "        lea     rax, [rax - offset]" << nl_ // TODO: offset
             << "        push    rax" << nl_;
}

void X86_64CodeGenerator::visit(const AssignmentNode *node) {
    *output_ << "        ; Begin evaluation for assignment" << nl_
             << "        push    rbp                     ; push base address" << nl_;

    node->getValue()->visit(this); // compute the value of the assignment

    *output_ << "        ; Begin address computation for assignment" << nl_;

    node->getAssignee()->visit(this); // compute the assignment address

    *output_ << "        ; Actual Assignment" << nl_
             << "        pop     rbx                     ; address" << nl_
             << "        pop     rax                     ; value" << nl_
             << "        mov     rbx, rax                ; perform the assignment" << nl_;
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

void X86_64CodeGenerator::visit(const NumberConstantNode *node) {
    *output_ << "        push    0x"
             << std::setw(16) << std::setfill('0') << std::hex << node->getValue()
             << "      ; Number constant " << node->getValue() << nl_;
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

std::string X86_64CodeGenerator::padRight(const std::string &value, const size_t column) {
    const auto pad_string = std::string(value.size() > column ? 0 : column - value.size(), ' ');

    return value + pad_string;
}