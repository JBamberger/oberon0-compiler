#include "DeclarationListNode.h"

template <class T>
const std::string DeclListTypeTraits<T>::name = "DeclarationListNode";

template <>
struct DeclListTypeTraits<ConstantDeclarationNode> {
    static const std::string name;
};
const std::string DeclListTypeTraits<ConstantDeclarationNode>::name = "ConstantDeclarationList";

template <>
struct DeclListTypeTraits<TypedIdentifierListNode<VariableNode>> {
    static const std::string name;
};
const std::string DeclListTypeTraits<TypedIdentifierListNode<VariableNode>>::name =
    "VariableDeclarationList";

template <>
struct DeclListTypeTraits<TypeDeclarationNode> {
    static const std::string name;
};
const std::string DeclListTypeTraits<TypeDeclarationNode>::name = "TypeDeclarationList";

template <>
struct DeclListTypeTraits<TypedIdentifierListNode<ParameterNode>> {
    static const std::string name;
};
const std::string DeclListTypeTraits<TypedIdentifierListNode<ParameterNode>>::name =
    "FormalParameterList";
