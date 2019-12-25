#pragma once
#include "ExpressionNode.h"
#include "TypeNode.h"

class ArrayTypeNode : public TypeNode {
    int size_;
    std::unique_ptr<TypeNode> type_;

  public:
    ArrayTypeNode(const FilePos& pos,
                  int size,
                  std::unique_ptr<TypeNode> type);
    ~ArrayTypeNode() override;
    int getSize() const;
    const std::unique_ptr<TypeNode>& getType() const;

    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
