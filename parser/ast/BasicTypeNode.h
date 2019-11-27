#pragma once
#include "TypeNode.h"

class BasicTypeNode : public TypeNode {
    std::string name_;

  public:
    BasicTypeNode(const FilePos& pos, std::string name);
    ~BasicTypeNode() override;

    void print(std::ostream& stream) const override;
};
