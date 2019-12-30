#pragma once
#include "TypeNode.h"

class ArrayTypeNode : public TypeNode {
    int size_;
    std::string type_;

  public:
    ArrayTypeNode(const FilePos& pos, int size, std::string type);

    ~ArrayTypeNode() override;

    int getSize() const;

    std::string getType() const;

    std::string getId() const override;

    void visit(NodeVisitor* visitor) const override;

    void print(std::ostream& stream) const override;
};
