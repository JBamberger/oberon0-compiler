#include "DeclarationListNode.h"

template <class T>
const std::string DeclListTypeTraits<T>::name = "DeclarationListNode";

template <>
struct DeclListTypeTraits<ConstantDeclarationNode> {
    static const std::string name;
};
const std::string DeclListTypeTraits<ConstantDeclarationNode>::name = "ConstantDeclarationList";

template <>
struct DeclListTypeTraits<VariableListNode> {
    static const std::string name;
};
const std::string DeclListTypeTraits<VariableListNode>::name = "VariableDeclarationList";

template <>
struct DeclListTypeTraits<TypeDeclarationNode> {
    static const std::string name;
};
const std::string DeclListTypeTraits<TypeDeclarationNode>::name = "TypeDeclarationList";

template <>
struct DeclListTypeTraits<ParameterListNode> {
    static const std::string name;
};
const std::string DeclListTypeTraits<ParameterListNode>::name = "FormalParameterList";
