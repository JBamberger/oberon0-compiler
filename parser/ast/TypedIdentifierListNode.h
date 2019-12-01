#pragma once
#include "FieldNode.h"
#include "IdentifierListNode.h"
#include "Node.h"
#include "ParameterNode.h"
#include "TypeNode.h"
#include "TypedIdentifierNode.h"
#include "VariableNode.h"
#include <vector>

template <class T>
class TypedIdentifierListNode : public Node {
    std::shared_ptr<std::vector<std::unique_ptr<const T>>> pairs_;
    std::shared_ptr<const TypeNode> type_;

  public:
    TypedIdentifierListNode(const FilePos& pos, const TypeNode* type)
        : Node(NodeType::typed_id_list, pos),
          pairs_(std::make_shared<std::vector<std::unique_ptr<const T>>>()), type_(type)
    {
        assert(type_ != nullptr);
    }

    ~TypedIdentifierListNode() override = default;

    std::shared_ptr<const TypeNode> getType() const { return type_; }

    std::shared_ptr<std::vector<std::unique_ptr<const T>>> getPairs() const { return pairs_; }

    void print(std::ostream& stream) const override
    {
        stream << "FieldListNode({";
        if (!pairs_->empty()) {
            stream << *(pairs_->at(0));
            for (auto i = 1; i < pairs_->size(); ++i) { stream << ", " << *(pairs_->at(i)); }
        }
        stream << "}, " << *type_ << ")";
    }

    friend const TypedIdentifierListNode<FieldNode>*
    createFieldList(const FilePos& pos, const IdentifierListNode* names, const TypeNode* type);

    friend const TypedIdentifierListNode<VariableNode>*
    createVariableList(const FilePos& pos, const IdentifierListNode* names, const TypeNode* type);

    friend const TypedIdentifierListNode<ParameterNode>*
    createParameterList(const FilePos& pos,
                        const IdentifierListNode* names,
                        const TypeNode* type,
                        bool is_reference);
};

inline const TypedIdentifierListNode<FieldNode>*
createFieldList(const FilePos& pos, const IdentifierListNode* names, const TypeNode* type)
{
    auto list = new TypedIdentifierListNode<FieldNode>(pos, type);
    for (const auto& name : names->getNames()) {
        const auto fieldType = new TypeReferenceNode(type->getFilePos(), list->type_);
        list->pairs_->push_back(
            std::make_unique<FieldNode>(fieldType->getFilePos(), name, fieldType));
    }

    return list;
}

inline const TypedIdentifierListNode<VariableNode>*
createVariableList(const FilePos& pos, const IdentifierListNode* names, const TypeNode* type)
{
    auto list = new TypedIdentifierListNode<VariableNode>(pos, type);
    for (const auto& name : names->getNames()) {
        const auto fieldType = new TypeReferenceNode(type->getFilePos(), list->type_);
        list->pairs_->push_back(
            std::make_unique<VariableNode>(fieldType->getFilePos(), name, fieldType));
    }

    return list;
}

inline const TypedIdentifierListNode<ParameterNode>* createParameterList(
    const FilePos& pos, const IdentifierListNode* names, const TypeNode* type, bool is_reference)
{
    auto list = new TypedIdentifierListNode<ParameterNode>(pos, type);
    for (const auto& name : names->getNames()) {
        const auto fieldType = new TypeReferenceNode(type->getFilePos(), list->type_);
        list->pairs_->push_back(std::make_unique<ParameterNode>(fieldType->getFilePos(), name,
                                                                fieldType, is_reference));
    }

    return list;
}