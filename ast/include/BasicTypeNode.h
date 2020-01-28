#pragma once
#include "TypeNode.h"

class BasicTypeNode : public TypeNode {

    std::string name_;
    size_t size_;

  public:
    BasicTypeNode(std::string name, size_t size);
    ~BasicTypeNode() override;
    const std::string& getName() const;
    std::string getId() const override;
    size_t getByteSize() const override;
    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;

    static std::unique_ptr<BasicTypeNode> makeInt();
    static std::unique_ptr<BasicTypeNode> makeBool();
    static std::unique_ptr<BasicTypeNode> makeString();
};