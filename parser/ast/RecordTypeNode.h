#pragma once
#include "FieldNode.h"
#include "TypeNode.h"
#include "TypedIdentifierListNode.h"
#include <vector>

class RecordTypeNode : public TypeNode {
    std::vector<std::shared_ptr<const TypeNode>> types_;
    std::vector<std::unique_ptr<const FieldNode>> members_;

  public:
    RecordTypeNode(const FilePos& pos);
    ~RecordTypeNode() override;
    void addFields(const TypedIdentifierListNode<FieldNode>* fields);
    void print(std::ostream& stream) const override;
};
