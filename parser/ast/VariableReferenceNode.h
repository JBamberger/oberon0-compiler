#pragma once
#include "ChainedReferenceNode.h"

class VariableReferenceNode : public ChainedReferenceNode {

    std::string name_;

  public:
    VariableReferenceNode(const FilePos& pos, std::string name);
    ~VariableReferenceNode() override;

    void print(std::ostream& stream) const override;
};
