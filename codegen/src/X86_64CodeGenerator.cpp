
#include "X86_64CodeGenerator.h"
#include "ArrayReferenceNode.h"
#include "ArrayTypeNode.h"
#include "AssignmentNode.h"
#include "BinaryExpressionNode.h"
#include "FieldReferenceNode.h"
#include "NumberConstantNode.h"
#include "RecordTypeNode.h"
#include "StringConstantNode.h"
#include "UnaryExpressionNode.h"
#include <iomanip>

X86_64CodeGenerator::X86_64CodeGenerator() : output_(nullptr), nl_("\n"), label_nr_(1)
{
}

X86_64CodeGenerator::~X86_64CodeGenerator() = default;

std::string X86_64CodeGenerator::nextLabel()
{
    return std::to_string(label_nr_++);
}

void X86_64CodeGenerator::generate(std::unique_ptr<ModuleNode> ast, std::ostream* output)
{
    output_ = output;
    ast->visit(this);
    output_ = nullptr;
}

// clang-format off

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
    const auto size = node->getVariables().getSize();
    *output_ << "        sub     rsp, " << size << "     ; reserve local variable space" << nl_;
    //TODO: initialize variables with zero?
}

void X86_64CodeGenerator::visit(const ModuleNode *node) {
    root_scope = node->getScope();


    *output_ << "%pragma macho64 prefix _" << nl_
             << nl_;

    *output_ << "%macro print 2" << nl_
             << "    %ifidn __OUTPUT_FORMAT__, elf64" << nl_
             << "        mov     rsi, %1                 ; first argument, message" << nl_
             << "        mov     rdi, %2                 ; second argument, integer" << nl_
             << "        xor     rax, rax                ; zero result" << nl_
             << "        call    printf                  ; print the formatted string" << nl_
             << "    %elifidn __OUTPUT_FORMAT__, win64" << nl_
             << "        sub     rsp, 40                 ; 32b shadow space + 8b alignment" << nl_
             << "        mov     rcx, %2                 ; first argument, message" << nl_
             << "        mov     rdx, %1                 ; second argument, integer" << nl_
             << "        xor     rax, rax                ; zero result" << nl_
             << "        call    printf                  ; print the formatted string" << nl_
             << "        add     rsp, 40                 ; remove shadow space and alignment" << nl_
             << "    %endif" << nl_
             << "%endmacro" << nl_;

    *output_ << nl_
             << "%macro rtstackalign 1" << nl_
             << "        mov     rax, rsp                ;" << nl_
             << "        mov     rbx, 16                 ;" << nl_
             << "        xor     rdx, rdx                ;" << nl_
             << "        cqo                             ;" << nl_
             << "        idiv    rbx                     ;" << nl_
             << "        cmp     rdx, 0                  ;" << nl_
             << "        je      %1                      ;" << nl_
             << "        add     rax, 16                 ;" << nl_
             << "        xor     rdx, rdx                ;" << nl_
             << "        cqo                             ;" << nl_
             << "        idiv    rbx                     ;" << nl_
             << "        imul    rax, 16                 ;" << nl_
             << "        mov     rsp, rax                ;" << nl_
             << "%1:                                     ;" << nl_
             << "        nop                             ;" << nl_
             << "%endmacro" << nl_
             << nl_
             << "        global  main                    ; program entry point" << nl_
             << "        extern  printf                  ; libc printf(string, args)" << nl_
             << nl_
             << "section .data" << nl_
             << "dmsg:" << nl_
             << "        db  'Debug output %d', 10, 0" << nl_
             << "eDivByZero:" << nl_
             << "        db 'Error: Division by zero.', 10, 0" << nl_
             << "eOutOfBounds:" << nl_
             << "        db 'Error: Array index out of bounds.', 10, 0" << nl_
             << nl_;
    defineConstants(node);
    *output_ << nl_
             << "section .text" << nl_
             << "main:                                   ; Module begin: " << node->getName() << nl_
             << "        push    rbp                     ; save caller stack frame" << nl_
             << "        mov     rbp, rsp                ; move to next stack frame" << nl_;
    defineVariables(node);
    // here goes the module body

    for (const auto &s : node->getStatements()) {
        s->visit(this);
    }

    *output_ << nl_
             << "        xor     rax, rax                ; set return value to 0" << nl_
             << "        mov     rsp, rbp                ; reset stack pointer for caller" << nl_
             << "        pop     rbp                     ; reset stack base for caller" << nl_
             << "        ret                             ; Module end: " << node->getName() << nl_
             // here go the modules functions
             << nl_
             << "DivByZero:                              ; Division by zero handler" << nl_
             << "        rtstackalign DivByZeroAlign" << nl_
             << "        print   0, eDivByZero           ; there is no second argument" << nl_
             << "        mov     rsp, rbp                ; reset stack pointer" << nl_
             << "        pop     rbp                     ; reset base pointer"
             << "        ret                             ; exit" << nl_
             << nl_
             << "OutOfBounds:" << nl_
             << "        rtstackalign OutOfBoundsAlign" << nl_
             << "        print   0, eOutOfBounds         ; there is no second argument" << nl_
             << "        mov     rsp, rbp                ; reset stack pointer" << nl_
             << "        pop     rbp                     ; reset base pointer"
             << "        ret                             ; exit" << nl_
             << nl_;


    root_scope = nullptr;
}

void X86_64CodeGenerator::visit(const ConstantDeclarationNode *node) {
    const auto label = node->getName() + nextLabel();
    const auto type = node->getValue()->getType();
    if (type->getId() == "INTEGER") {
//        INTEGER constants are used as immediate value in the instructions
//        const auto value = dynamic_cast<NumberConstantNode *>(node->getValue().get())->getValue();
//        *output_ << padRight(label + ":", COMMENT_COL) << "; integer constant definition" << nl_
//                 << "        dq      " << std::to_string(value) << nl_;
    } else if (type->getId() == "STRING") {
        //TODO: escape the string appropriately
        const auto value = dynamic_cast<StringConstantNode *>(node->getValue().get())->getValue();
        *output_ << std::setw(40) << (label + ":") << "; string constant definition" << nl_
                 << "        db      '" << value << "', 0" << nl_;
    } else {
        throw std::runtime_error("Only integer constants are supported.");
    }

    //TODO: save label name of the constant
}

void X86_64CodeGenerator::visit(const ArrayReferenceNode *node) {
    bool deref = should_deref;
    const auto array = dynamic_cast<ArrayTypeNode*>(node->getArrayRef()->getType());
    assert(array != nullptr);
    const auto  elem_size = node->getType()->getByteSize();
    const auto array_len = static_cast<size_t>(array->getSize());
    *output_ << "        ; Array access index computation" << nl_;
    node->getIndex()->visit(this); // evaluates the index

    // compute array base address
    should_deref = false;
    node->getArrayRef()->visit(this);

    *output_ << "        ; Array value reference" << nl_
             << "        pop     rbx                     ; array base address" << nl_
             << "        pop     rax                     ; array index" << nl_
             << "        cmp     rax, 0                  ; bounds check lower" << nl_
             << "        jl      OutOfBounds             ; exit" << nl_
             << "        cmp     "
                     << std::setw(24) << ("rax, " + std::to_string(array_len))
                     << " ; bounds check upper" << nl_
             << "        jge     OutOfBounds             ; exit" << nl_;


    if (elem_size > 8) {
        *output_ << "        mov     rcx, " << elem_size << nl_
                 << "        imul    rax, rcx                ; offset too large for lea" << nl_
                 << "        lea     rbx, [rbx+rax]          ; array access" << nl_;
    } else {
        *output_ << "        lea     "
                         << std::setw(24) << "rbx, [rbx+rax*" + std::to_string(elem_size) + "]"
                         << "; array access" << nl_;
    }
    *output_ << "        push    rbx" << nl_;
    should_deref = true;
    if (!deref) return;
    *output_ << "        pop     rax" << nl_
             << "        mov     rbx, qword [rax]" << nl_
             << "        push    rbx" << nl_;
}

void X86_64CodeGenerator::visit(const VariableReferenceNode *node) {
    bool deref = should_deref;
    const auto var = node->getVariable();
    const auto offset = var->getParent()->getVariables().at(var->getName()).offset;
    *output_ << "        ; Variable reference " << var->getName() << nl_
             << "        lea     rax, [rbp - "<<  offset << "]" << nl_
             << "        push    rax" << nl_;

    should_deref = true;
    if (!deref) return;
    *output_ << "        pop     rax" << nl_
             << "        mov     rbx, qword [rax]" << nl_
             << "        push    rbx" << nl_;
}

void X86_64CodeGenerator::visit(const FieldReferenceNode *node) {
    bool deref = should_deref;
    const auto record = dynamic_cast<RecordTypeNode*>(node->getRecordRef()->getType());
    assert(record != nullptr);
    const auto offset = record->getMembers().at(node->getFieldName()).offset;

    // compute record base address
    should_deref = false;
    node->getRecordRef()->visit(this);

    *output_ << "        ; Record field reference " << node->getFieldName() << nl_
             << "        pop     rax" << nl_
             << "        lea     rax, [rax + "<< offset <<"]" << nl_
             << "        push    rax" << nl_;
    should_deref = true;
    if (!deref) return;
    *output_ << "        pop     rax" << nl_
             << "        mov     rbx, qword [rax]" << nl_
             << "        push    rbx" << nl_;
}

void X86_64CodeGenerator::visit(const AssignmentNode *node) {
    *output_ << "        ; "; node->print(*output_); *output_ << nl_;
    *output_ << "        ; Compute R-Value" << nl_;

    node->getValue()->visit(this);

    *output_ << "        ; Compute L-Value" << nl_;

    should_deref = false;
    node->getAssignee()->visit(this); // compute the assignment address

    *output_ << "        ; Actual Assignment" << nl_
             << "        pop     rbx                     ; address" << nl_
             << "        pop     rax                     ; value" << nl_
             << "        mov     qword [rbx], rax        ; perform the assignment" << nl_
             << "        print   [rbx], dmsg             ; DEBUG PRINT STATEMENT" << nl_;
}

void X86_64CodeGenerator::visit(const BinaryExpressionNode *node) {
    // push op 1 to the stack
    node->getOperand1()->visit(this);
    // push op 2 to the stack
    node->getOperand2()->visit(this);

    switch (node->getOperator()) {
        case BinaryOperator::plus:
            *output_ << "        ; addition" << nl_
                     << "        pop     rbx" << nl_
                     << "        pop     rax" << nl_
                     << "        add     rax, rbx" << nl_
                     << "        push    rax" << nl_;
            break;
        case BinaryOperator::minus:
            *output_ << "        ; subtraction" << nl_
                     << "        pop     rbx" << nl_
                     << "        pop     rax" << nl_
                     << "        sub     rax, rbx" << nl_
                     << "        push    rax" << nl_;
            break;
        case BinaryOperator::times:
            *output_ << "        ; multiplication" << nl_
                     << "        pop     rbx" << nl_
                     << "        pop     rax" << nl_
                     << "        imul    rax, rbx                ; perform signed multiplication" << nl_
                     << "        push    rax" << nl_;
            break;
        case BinaryOperator::div:
            *output_ << "        ; signed integer division" << nl_
                     << "        pop     rbx                     ; denominator" << nl_
                     << "        pop     rax                     ; numerator" << nl_
                     << "        cmp     rbx, 0                  ; check denominator for Zero" << nl_
                     << "        je      DivByZero               ; error out if zero" << nl_
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
                     << "        pop     rbx                     ; denominator" << nl_
                     << "        pop     rax                     ; numerator" << nl_
                     << "        cmp     rbx, 0                  ; check denominator for Zero" << nl_
                     << "        je      DivByZero               ; error out if zero" << nl_
                     << "        xor     rdx, rdx                ; zero out rdx" << nl_
                     << "        cqo                             ; build rdx:rax by sign extension" << nl_
                     << "        idiv    rbx                     ; perform signed division" << nl_
                     << "        mov     rax, rdx                ; set return value" << nl_
                     << "        test    rdx, rdx                ; sets sign bit if negative" << nl_
                     << "        jns     "<< std::setw(24) << jumpLabel << "; finished if non-zero" << nl_
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
                     << "        pop     rcx" << nl_
                     << "        pop     rbx" << nl_
                     << "        xor     eax, eax" << nl_
                     << "        or      rbx, rcx" << nl_
                     << "        setne   al" << nl_
                     << "        push    rax" << nl_;
            break;
        case BinaryOperator::logical_and:
            *output_ << "        ; logical and" << nl_
                     << "        pop     rcx" << nl_
                     << "        pop     rbx" << nl_
                     << "        test    rbx, rbx                ; make first operand to real bool" << nl_
                     << "        setne   dl                      ; rdx is now real bool" << nl_
                     << "        xor     eax, eax                ; zero out rdx" << nl_
                     << "        test    rcx, rcx                ; make second operand real bool" << nl_
                     << "        setne   al                      ; rax is now real bool" << nl_
                     << "        and     rax, rdx                ; a && b" << nl_
                     << "        push    rax" << nl_;
            break;
        default:
            std::string op;
            std::string name;
            switch (node->getOperator()) {
                case BinaryOperator::eq:
                    op = "sete";
                    name = "equals";
                    break;
                case BinaryOperator::neq:
                    op = "setne";
                    name = "not equals";
                    break;
                case BinaryOperator::lt:
                    op = "setl";
                    name = "less than";
                    break;
                case BinaryOperator::leq:
                    op = "setle";
                    name = "less than or equals";
                    break;
                case BinaryOperator::gt:
                    op = "setg";
                    name = "greater than";
                    break;
                case BinaryOperator::geq:
                    op = "setge";
                    name = "greater than or equals";
                    break;
                default:
                    throw std::runtime_error("Unsupported operation!");
            }
            *output_ << "        ; " << name << nl_
                     << "        pop     rcx" << nl_
                     << "        pop     rbx" << nl_
                     << "        xor     eax, eax                ; zero out rdx" << nl_
                     << "        comp    rbx, rcx                ; compare a and b" << nl_
                     << "        " << std::setw(8) << op << std::setw(24) << "al" << "; set rax to result" << nl_
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

void X86_64CodeGenerator::visit(const NumberConstantNode *node) {
    *output_ << "        push    "
             << std::setw(24) << std::setfill(' ') <<std::left << node->getValue()
             << "; Number constant " << node->getValue() << nl_;
}

void X86_64CodeGenerator::visit(const StringConstantNode *) {
    throw std::runtime_error("String constants are not supported.");
}




void X86_64CodeGenerator::visit(const TypedIdentifierNode *) {
}

void X86_64CodeGenerator::visit(const VariableDeclarationNode *) {
}

void X86_64CodeGenerator::visit(const FieldDeclarationNode *) {
}

void X86_64CodeGenerator::visit(const ParameterDeclarationNode *) {
}

void X86_64CodeGenerator::visit(const ProcedureDeclarationNode *) {
}

void X86_64CodeGenerator::visit(const ProcedureCallNode *) {
}

void X86_64CodeGenerator::visit(const IfStatementNode *) {
}

void X86_64CodeGenerator::visit(const WhileStatementNode *) {
}

// clang-format on
