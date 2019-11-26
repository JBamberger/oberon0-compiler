#pragma once
#include "VariableReferenceNode.h"

class MemberReferenceNode : public VariableReferenceNode {

    std::string name_;

  public:
    MemberReferenceNode(const FilePos& pos, std::string name, const VariableReferenceNode* next);
    virtual ~MemberReferenceNode() override;

    void print(std::ostream& stream) const override;
};