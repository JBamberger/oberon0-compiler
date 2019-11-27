#pragma once
#include "IdentifierListNode.h"
#include "Node.h"
#include "TypeNode.h"
#include <vector>

class FieldListNode : public Node {
    std::unique_ptr<const IdentifierListNode> names_;
    std::shared_ptr<const TypeNode> type_;

  public:
    FieldListNode(const FilePos& pos, const IdentifierListNode* names, const TypeNode* type);
    ~FieldListNode() override;
    std::shared_ptr<const TypeNode> getType() const;
    const IdentifierListNode* getNames() const;
    void print(std::ostream& stream) const override;
};
