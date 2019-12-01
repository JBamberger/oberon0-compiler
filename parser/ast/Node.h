/*
 * Header file of the base class of all AST nodes used by the Oberon-0 compiler.
 *
 * Created by Michael Grossniklaus on 2/2/18.
 */

#ifndef OBERON0C_AST_H
#define OBERON0C_AST_H

#include "../../util/Logger.h"
#include <ostream>

enum class NodeType : char {
    unary_expression,
    binary_expression,
    constant_reference,
    boolean_constant,
    number_constant,
    string_constant,
    type_reference,
    record_type,
    array_type,
    basic_type,
    field,
    parameter,
    variable,
    variable_reference,
    module,
    procedure,
    while_statement,
    if_statement,
    procedure_call,
    assignment,
    statement_sequence,
    id_list,
    typed_id_list,
    declarations,
    declaration_list,
    type_declaration,
    procedure_heading,
    procedure_body,
    procedure_declaration,
    constant_declaration
};

class Node {
    NodeType nodeType_;
    FilePos pos_;

  public:
    explicit Node(NodeType nodeType, FilePos pos);
    virtual ~Node() = 0;

    const NodeType getNodeType() const;
    const FilePos getFilePos() const;

    virtual void print(std::ostream& stream) const = 0;
    friend std::ostream& operator<<(std::ostream& stream, const Node& node);
};

#endif // OBERON0C_AST_H