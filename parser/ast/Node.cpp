#include "Node.h"
#include <utility>

Node::Node(const NodeType nodeType, FilePos pos) : nodeType_(nodeType), pos_(std::move(pos)) {}

Node::~Node() = default;

NodeType Node::getNodeType() const { return nodeType_; }

FilePos Node::getFilePos() const { return pos_; }

std::ostream& operator<<(std::ostream& stream, const NodeType& nt)
{
    switch (nt) {
    case NodeType::unary_expression:
        return stream << "unary_expression";
    case NodeType::binary_expression:
        return stream << "binary_expression";
    case NodeType::constant_reference:
        return stream << "constant_reference";
    case NodeType::boolean_constant:
        return stream << "boolean_constant";
    case NodeType::number_constant:
        return stream << "number_constant";
    case NodeType::string_constant:
        return stream << "string_constant";
    case NodeType::type_reference:
        return stream << "type_reference";
    case NodeType::record_type:
        return stream << "record_type";
    case NodeType::array_type:
        return stream << "array_type";
    case NodeType::basic_type:
        return stream << "basic_type";
    case NodeType::field:
        return stream << "field";
    case NodeType::parameter:
        return stream << "parameter";
    case NodeType::variable:
        return stream << "variable";
    case NodeType::variable_reference:
        return stream << "variable_reference";
    case NodeType::module:
        return stream << "module";
    case NodeType::procedure:
        return stream << "procedure";
    case NodeType::while_statement:
        return stream << "while_statement";
    case NodeType::if_statement:
        return stream << "if_statement";
    case NodeType::procedure_call:
        return stream << "procedure_call";
    case NodeType::assignment:
        return stream << "assignment";
    case NodeType::statement_sequence:
        return stream << "statement_sequence";
    case NodeType::id_list:
        return stream << "id_list";
    case NodeType::typed_id_list:
        return stream << "typed_id_list";
    case NodeType::declarations:
        return stream << "declarations";
    case NodeType::declaration_list:
        return stream << "declaration_list";
    case NodeType::type_declaration:
        return stream << "type_declaration";
    case NodeType::procedure_heading:
        return stream << "procedure_heading";
    case NodeType::procedure_body:
        return stream << "procedure_body";
    case NodeType::procedure_declaration:
        return stream << "procedure_declaration";
    case NodeType::constant_declaration:
        return stream << "constant_declaration";
    case NodeType::selector:
        return stream << "selector";
    default:
        std::cerr << "unknown node type " << nt << std::endl;
        exit(EXIT_FAILURE);
    }
}

std::ostream& operator<<(std::ostream& stream, const Node& node)
{
    node.print(stream);
    return stream;
}
