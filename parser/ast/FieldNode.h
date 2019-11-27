#pragma once
#include "Node.h"
#include "TypeReferenceNode.h"

class FieldNode : public Node {
    std::string name_;
    std::unique_ptr<const TypeReferenceNode> type_;

  public:
    FieldNode(const FilePos& pos, std::string name, const TypeReferenceNode* type);
    ~FieldNode() override;

    void print(std::ostream& stream) const override;
};
