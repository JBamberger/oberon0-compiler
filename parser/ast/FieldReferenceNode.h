#pragma once
#include "SelectorNode.h"

class FieldReferenceNode : public SelectorNode {

    std::string name_;

  public:
    FieldReferenceNode(const FilePos& pos, std::string name);
    ~FieldReferenceNode() override;

    const std::string& getName() const;

    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
