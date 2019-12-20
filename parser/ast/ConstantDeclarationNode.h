#pragma once
#include "ExpressionNode.h"
#include "Node.h"

class ConstantDeclarationNode : public Node {
    std::string name_;
    std::unique_ptr<ExpressionNode> value_;

  public:
    ConstantDeclarationNode(const FilePos& pos,
                            std::string name,
                            std::unique_ptr<ExpressionNode> value);
    ~ConstantDeclarationNode() override;

    const std::string& getName() const;
    const std::unique_ptr<ExpressionNode>& getValue() const;

    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
