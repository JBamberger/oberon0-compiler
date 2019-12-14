#pragma once
#include "SelectorNode.h"

class FieldReferenceNode : public SelectorNode {

    std::string name_;

  public:
    FieldReferenceNode(const FilePos& pos, std::string name);
    ~FieldReferenceNode() override;

    void print(std::ostream& stream) const override;
};
