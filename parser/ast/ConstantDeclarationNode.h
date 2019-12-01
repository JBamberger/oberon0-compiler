#pragma once
#include "Node.h"
#include "ExpressionNode.h"

class ConstantDeclarationNode : public Node {
    std::string name_;
    std::unique_ptr<const ExpressionNode> value_;

public:
    ConstantDeclarationNode(const FilePos& pos, std::string name, const ExpressionNode* value);
    ~ConstantDeclarationNode() override;

    void print(std::ostream& stream) const override;
};