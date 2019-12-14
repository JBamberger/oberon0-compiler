#pragma once
#include "ExpressionNode.h"
#include "SelectorNode.h"

class VariableReferenceNode : public ExpressionNode {

    std::string name_;
    std::unique_ptr<const SelectorNode> selector_;

  public:
    VariableReferenceNode(const FilePos& pos, std::string name);
    ~VariableReferenceNode() override;
    void setSelector(const SelectorNode* node);
    void print(std::ostream& stream) const override;
};
