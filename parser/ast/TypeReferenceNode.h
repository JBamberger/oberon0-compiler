#pragma once
#include "Node.h"
#include "TypeNode.h"

class TypeReferenceNode : public Node {
    std::shared_ptr<const TypeNode> type_;

  public:
    TypeReferenceNode(const FilePos& pos, std::shared_ptr<const TypeNode> type);
    virtual ~TypeReferenceNode() override;

    void print(std::ostream& stream) const override;
};
