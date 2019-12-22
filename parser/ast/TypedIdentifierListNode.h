#pragma once
#include "FieldNode.h"
#include "IdentifierListNode.h"
#include "Node.h"
#include "NodeVisitor.h"
#include "ParameterNode.h"
#include "TypeNode.h"
#include "TypedIdentifierNode.h"
#include "VariableDeclarationNode.h"
#include <vector>

class FieldListNode : public Node {
    std::shared_ptr<std::vector<std::unique_ptr<FieldNode>>> pairs_;
    std::shared_ptr<TypeNode> type_;

  public:
    FieldListNode(const FilePos& pos, std::unique_ptr<TypeNode> type)
        : Node(NodeType::typed_id_list, pos),
          pairs_(std::make_shared<std::vector<std::unique_ptr<FieldNode>>>()),
          type_(std::move(type))
    {
        assert(type_ != nullptr);
    }

    ~FieldListNode() override = default;

    std::shared_ptr<TypeNode> getType() const { return type_; }

    std::shared_ptr<std::vector<std::unique_ptr<FieldNode>>> getPairs() const { return pairs_; }

    std::unique_ptr<TypeReferenceNode> getTypeRef() const
    {
        return std::make_unique<TypeReferenceNode>(type_->getFilePos(), type_);
    }

    void visit(NodeVisitor* visitor) const override { visitor->visit(this); }

    void print(std::ostream& stream) const override
    {
        stream << "FieldListNode({";
        if (!pairs_->empty()) {
            stream << *(pairs_->at(0));
            for (size_t i = 1; i < pairs_->size(); ++i) {
                stream << ", " << *(pairs_->at(i));
            }
        }
        stream << "}, " << *type_ << ")";
    }
};

class VariableListNode : public Node {
    std::shared_ptr<std::vector<std::unique_ptr<VariableDeclarationNode>>> pairs_;
    std::shared_ptr<TypeNode> type_;

  public:
    VariableListNode(const FilePos& pos, std::unique_ptr<TypeNode> type)
        : Node(NodeType::typed_id_list, pos),
          pairs_(std::make_shared<std::vector<std::unique_ptr<VariableDeclarationNode>>>()),
          type_(std::move(type))
    {
        assert(type_ != nullptr);
    }

    ~VariableListNode() override = default;

    std::shared_ptr<TypeNode> getType() const { return type_; }

    std::shared_ptr<std::vector<std::unique_ptr<VariableDeclarationNode>>> getPairs() const { return pairs_; }

    std::unique_ptr<TypeReferenceNode> getTypeRef() const
    {
        return std::make_unique<TypeReferenceNode>(type_->getFilePos(), type_);
    }

    void visit(NodeVisitor* visitor) const override { visitor->visit(this); }

    void print(std::ostream& stream) const override
    {
        stream << "VariableListNode({";
        if (!pairs_->empty()) {
            stream << *(pairs_->at(0));
            for (size_t i = 1; i < pairs_->size(); ++i) {
                stream << ", " << *(pairs_->at(i));
            }
        }
        stream << "}, " << *type_ << ")";
    }
};

class ParameterListNode : public Node {
    std::shared_ptr<std::vector<std::unique_ptr<ParameterNode>>> pairs_;
    std::shared_ptr<TypeNode> type_;

  public:
    ParameterListNode(const FilePos& pos, std::unique_ptr<TypeNode> type)
        : Node(NodeType::typed_id_list, pos),
          pairs_(std::make_shared<std::vector<std::unique_ptr<ParameterNode>>>()),
          type_(std::move(type))
    {
        assert(type_ != nullptr);
    }

    ~ParameterListNode() override = default;

    std::shared_ptr<TypeNode> getType() const { return type_; }

    std::shared_ptr<std::vector<std::unique_ptr<ParameterNode>>> getPairs() const { return pairs_; }

    std::unique_ptr<TypeReferenceNode> getTypeRef() const
    {
        return std::make_unique<TypeReferenceNode>(type_->getFilePos(), type_);
    }

    void visit(NodeVisitor* visitor) const override { visitor->visit(this); }

    void print(std::ostream& stream) const override
    {
        stream << "ParameterListNode({";
        if (!pairs_->empty()) {
            stream << *(pairs_->at(0));
            for (size_t i = 1; i < pairs_->size(); ++i) {
                stream << ", " << *(pairs_->at(i));
            }
        }
        stream << "}, " << *type_ << ")";
    }
};

inline FieldListNode*
createFieldList(const FilePos& pos, const IdentifierListNode* names, std::unique_ptr<TypeNode> type)
{
    const auto list = new FieldListNode(pos, std::move(type));
    for (const auto& name : names->getNames()) {
        list->getPairs()->push_back(std::make_unique<FieldNode>(pos, name, list->getTypeRef()));
    }

    return list;
}

inline VariableListNode* createVariableList(const FilePos& pos,
                                            const IdentifierListNode* names,
                                            std::unique_ptr<TypeNode> type)
{
    const auto list = new VariableListNode(pos, std::move(type));
    for (const auto& name : names->getNames()) {
        list->getPairs()->push_back(std::make_unique<VariableDeclarationNode>(pos, name, list->getTypeRef()));
    }

    return list;
}

inline ParameterListNode* createParameterList(const FilePos& pos,
                                              const IdentifierListNode* names,
                                              std::unique_ptr<TypeNode> type,
                                              bool is_reference)
{
    const auto list = new ParameterListNode(pos, std::move(type));
    for (const auto& name : names->getNames()) {
        list->getPairs()->push_back(
            std::make_unique<ParameterNode>(pos, name, list->getTypeRef(), is_reference));
    }

    return list;
}