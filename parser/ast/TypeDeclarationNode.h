#pragma once
#include "Node.h"
#include "TypeNode.h"

class TypeDeclarationNode : public Node {
    std::string name_;
    std::string type_;

  public:
    TypeDeclarationNode(const FilePos& pos, std::string name, std::string type);
    ~TypeDeclarationNode() override;

    std::string getName() const;
    std::string getType() const;
    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
