#pragma once
#include "Node.h"
#include "TypeNode.h"

class TypeReferenceNode : public Node {
    std::shared_ptr<const TypeNode> type_;

  public:
    TypeReferenceNode(const FilePos& pos, std::shared_ptr<const TypeNode> type);
    ~TypeReferenceNode() override;

    const std::shared_ptr<const TypeNode>& getType() const;
    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
