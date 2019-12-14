#pragma once
#include "FieldNode.h"
#include "IdentifierListNode.h"
#include "Node.h"
#include "NodeVisitor.h"
#include "ParameterNode.h"
#include "TypeNode.h"
#include "TypedIdentifierNode.h"
#include "VariableNode.h"
#include <vector>

class FieldListNode : public Node {
    std::shared_ptr<std::vector<std::unique_ptr<const FieldNode>>> pairs_;
    std::shared_ptr<const TypeNode> type_;

  public:
    FieldListNode(const FilePos& pos, const TypeNode* type)
        : Node(NodeType::typed_id_list, pos),
          pairs_(std::make_shared<std::vector<std::unique_ptr<const FieldNode>>>()), type_(type)
    {
        assert(type_ != nullptr);
    }

    ~FieldListNode() override = default;

    std::shared_ptr<const TypeNode> getType() const { return type_; }

    std::shared_ptr<std::vector<std::unique_ptr<const FieldNode>>> getPairs() const
    {
        return pairs_;
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

    friend const FieldListNode*
    createFieldList(const FilePos& pos, const IdentifierListNode* names, const TypeNode* type)
    {
        auto list = new FieldListNode(pos, type);
        for (const auto& name : names->getNames()) {
            const auto fieldType = new TypeReferenceNode(type->getFilePos(), list->type_);
            list->pairs_->push_back(
                std::make_unique<FieldNode>(fieldType->getFilePos(), name, fieldType));
        }

        return list;
    }
};

class VariableListNode : public Node {
    std::shared_ptr<std::vector<std::unique_ptr<const VariableNode>>> pairs_;
    std::shared_ptr<const TypeNode> type_;

  public:
    VariableListNode(const FilePos& pos, const TypeNode* type)
        : Node(NodeType::typed_id_list, pos),
          pairs_(std::make_shared<std::vector<std::unique_ptr<const VariableNode>>>()), type_(type)
    {
        assert(type_ != nullptr);
    }

    ~VariableListNode() override = default;

    std::shared_ptr<const TypeNode> getType() const { return type_; }

    std::shared_ptr<std::vector<std::unique_ptr<const VariableNode>>> getPairs() const
    {
        return pairs_;
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

    friend const VariableListNode*
    createVariableList(const FilePos& pos, const IdentifierListNode* names, const TypeNode* type)
    {
        auto list = new VariableListNode(pos, type);
        for (const auto& name : names->getNames()) {
            const auto fieldType = new TypeReferenceNode(type->getFilePos(), list->type_);
            list->pairs_->push_back(
                std::make_unique<VariableNode>(fieldType->getFilePos(), name, fieldType));
        }

        return list;
    }
};

class ParameterListNode : public Node {
    std::shared_ptr<std::vector<std::unique_ptr<const ParameterNode>>> pairs_;
    std::shared_ptr<const TypeNode> type_;

  public:
    ParameterListNode(const FilePos& pos, const TypeNode* type)
        : Node(NodeType::typed_id_list, pos),
          pairs_(std::make_shared<std::vector<std::unique_ptr<const ParameterNode>>>()), type_(type)
    {
        assert(type_ != nullptr);
    }

    ~ParameterListNode() override = default;

    std::shared_ptr<const TypeNode> getType() const { return type_; }

    std::shared_ptr<std::vector<std::unique_ptr<const ParameterNode>>> getPairs() const
    {
        return pairs_;
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

    friend const ParameterListNode* createParameterList(const FilePos& pos,
                                                        const IdentifierListNode* names,
                                                        const TypeNode* type,
                                                        bool is_reference)
    {
        auto list = new ParameterListNode(pos, type);
        for (const auto& name : names->getNames()) {
            const auto fieldType = new TypeReferenceNode(type->getFilePos(), list->type_);
            list->pairs_->push_back(std::make_unique<ParameterNode>(fieldType->getFilePos(), name,
                                                                    fieldType, is_reference));
        }

        return list;
    }
};
