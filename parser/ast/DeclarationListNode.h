#pragma once
#include "ConstantDeclarationNode.h"
#include "DeclarationListNode.h"
#include "Node.h"
#include "TypeDeclarationNode.h"
#include "TypedIdentifierListNode.h"
#include <vector>

template <class T>
struct DeclListTypeTraits {
    static const std::string name;
};

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
        stream << DeclListTypeTraits<T>::name << "(";
        if (!list_.empty()) {
            stream << *list_.at(0);
            for (auto i = 1; i < list_.size(); ++i) { stream << ", " << *list_.at(i); }
        }
        stream << ")";
    }
};

using ConstantDeclarationList = DeclarationListNode<ConstantDeclarationNode>;
using VariableDeclarationList = DeclarationListNode<TypedIdentifierListNode<VariableNode>>;
using TypeDeclarationList = DeclarationListNode<TypeDeclarationNode>;
using FormalParameterList = DeclarationListNode<TypedIdentifierListNode<ParameterNode>>;
