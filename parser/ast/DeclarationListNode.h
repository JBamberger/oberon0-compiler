#pragma once
#include "ConstantDeclarationNode.h"
#include "DeclarationListNode.h"
#include "Node.h"
#include "TypeDeclarationNode.h"
#include "TypedIdentifierListNode.h"
#include <vector>

template <class T>
class DeclarationListNode : public Node {
    std::vector<std::unique_ptr<const T>> list_;

  public:
    DeclarationListNode(const FilePos& pos, std::vector<std::unique_ptr<const T>> list)
        : Node(NodeType::declaration_list, pos), list_(std::move(list))
    {
    }
    ~DeclarationListNode() override = default;

    void print(std::ostream& stream) const override
    {
        stream << "DeclarationListNode(";
        for (const auto& v : list_) stream << *v << " ";
        stream << ")";
    }
};

using ConstantDeclarationList = DeclarationListNode<ConstantDeclarationNode>;
using VariableDeclarationList = DeclarationListNode<VariableListNode>;
using TypeDeclarationList = DeclarationListNode<TypeDeclarationNode>;
using FormalParameterList = DeclarationListNode<ParameterListNode>;
