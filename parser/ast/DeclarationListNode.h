#pragma once
#include "ConstantDeclarationNode.h"
#include "DeclarationListNode.h"
#include "Node.h"
#include "NodeVisitor.h"
#include "TypeDeclarationNode.h"
#include "TypedIdentifierListNode.h"
#include <vector>

class ConstantDeclarationList : public Node {
    std::vector<std::unique_ptr<const ConstantDeclarationNode>> list_;

  public:
    ConstantDeclarationList(const FilePos& pos,
                            std::vector<std::unique_ptr<const ConstantDeclarationNode>> list)
        : Node(NodeType::declaration_list, pos), list_(std::move(list))
    {
    }

    ~ConstantDeclarationList() override = default;

    const std::vector<std::unique_ptr<const ConstantDeclarationNode>>& getList() const
    {
        return list_;
    }

    void visit(NodeVisitor* visitor) const override { visitor->visit(this); }

    void print(std::ostream& stream) const override
    {
        stream << "ConstantDeclarationList(";
        if (!list_.empty()) {
            stream << *list_.at(0);
            for (auto i = 1; i < list_.size(); ++i) {
                stream << ", " << *list_.at(i);
            }
        }
        stream << ")";
    }
};

class VariableDeclarationList : public Node {
    std::vector<std::unique_ptr<const VariableListNode>> list_;

  public:
    VariableDeclarationList(const FilePos& pos,
                            std::vector<std::unique_ptr<const VariableListNode>> list)
        : Node(NodeType::declaration_list, pos), list_(std::move(list))
    {
    }

    ~VariableDeclarationList() override = default;

    const std::vector<std::unique_ptr<const VariableListNode>>& getList() const { return list_; }

    void visit(NodeVisitor* visitor) const override { visitor->visit(this); }

    void print(std::ostream& stream) const override
    {
        stream << "VariableDeclarationList(";
        if (!list_.empty()) {
            stream << *list_.at(0);
            for (size_t i = 1; i < list_.size(); ++i) {
                stream << ", " << *list_.at(i);
            }
        }
        stream << ")";
    }
};

class TypeDeclarationList : public Node {
    std::vector<std::unique_ptr<const TypeDeclarationNode>> list_;

  public:
    TypeDeclarationList(const FilePos& pos,
                        std::vector<std::unique_ptr<const TypeDeclarationNode>> list)
        : Node(NodeType::declaration_list, pos), list_(std::move(list))
    {
    }

    ~TypeDeclarationList() override = default;

    const std::vector<std::unique_ptr<const TypeDeclarationNode>>& getList() const { return list_; }

    void visit(NodeVisitor* visitor) const override { visitor->visit(this); }

    void print(std::ostream& stream) const override
    {
        stream << "TypeDeclarationList(";
        if (!list_.empty()) {
            stream << *list_.at(0);
            for (size_t i = 1; i < list_.size(); ++i) {
                stream << ", " << *list_.at(i);
            }
        }
        stream << ")";
    }
};

class FormalParameterList : public Node {
    std::vector<std::unique_ptr<const ParameterListNode>> list_;

  public:
    FormalParameterList(const FilePos& pos,
                        std::vector<std::unique_ptr<const ParameterListNode>> list)
        : Node(NodeType::declaration_list, pos), list_(std::move(list))
    {
    }

    ~FormalParameterList() override = default;

    const std::vector<std::unique_ptr<const ParameterListNode>>& getList() const { return list_; }

    void visit(NodeVisitor* visitor) const override { visitor->visit(this); }

    void print(std::ostream& stream) const override
    {
        stream << "FormalParameterList(";
        if (!list_.empty()) {
            stream << *list_.at(0);
            for (size_t i = 1; i < list_.size(); ++i) {
                stream << ", " << *list_.at(i);
            }
        }
        stream << ")";
    }
};
