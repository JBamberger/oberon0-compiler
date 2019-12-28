#pragma once
#include "ExpressionNode.h"
#include "TypeNode.h"

class ArrayTypeNode : public TypeNode {
    int size_;
    std::shared_ptr<TypeNode> type_;

  public:
    ArrayTypeNode(const FilePos& pos, int size, std::shared_ptr<TypeNode> type);
    ~ArrayTypeNode() override;
    int getSize() const;
    std::shared_ptr<TypeNode> getType() const;

    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
