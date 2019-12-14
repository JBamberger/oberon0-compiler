#pragma once
#include "TypeNode.h"

class BasicTypeNode : public TypeNode {
    std::string name_;

  public:
    BasicTypeNode(const FilePos& pos, std::string name);

    ~BasicTypeNode() override;

    const std::string& getName() const;

    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
