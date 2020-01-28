#pragma once
#include "Node.h"
#include "TypeNode.h"

class TypeDeclarationNode : public Node {
    std::string name_;
    TypeNode* type_;

  public:
    TypeDeclarationNode(const FilePos& pos, std::string name, TypeNode* type);
    ~TypeDeclarationNode() override;

    std::string getName() const;
    TypeNode* getType() const;
    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
