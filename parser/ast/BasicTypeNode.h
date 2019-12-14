#pragma once
#include "TypeNode.h"

class BasicTypeNode : public TypeNode {
    std::string name_;

  public:
    BasicTypeNode(const FilePos& pos, std::string name);

    ~BasicTypeNode() override;

    const std::string& getName() const;

    void print(std::ostream& stream) const override;
};
