#pragma once
#include "ExpressionNode.h"
#include "Node.h"

class ConstantDeclarationNode : public Node {
    std::string name_;
    std::unique_ptr<const ExpressionNode> value_;

  public:
    ConstantDeclarationNode(const FilePos& pos, std::string name, const ExpressionNode* value);
    ~ConstantDeclarationNode() override;

    const std::string& getName() const;
    const std::unique_ptr<const ExpressionNode>& getValue() const;
    void print(std::ostream& stream) const override;
};
