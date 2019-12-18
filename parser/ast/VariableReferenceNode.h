#pragma once
#include "ExpressionNode.h"
#include "SelectorNode.h"

class VariableReferenceNode : public ExpressionNode {

    std::string name_;
    std::unique_ptr<const SelectorNode> selector_;

  public:
    VariableReferenceNode(const FilePos& pos, std::string name);
    VariableReferenceNode(const FilePos& pos, std::string name, SelectorNode * selector);
    ~VariableReferenceNode() override;
    void setSelector(const SelectorNode* node);

    const std::string& getName() const;
    const std::unique_ptr<const SelectorNode>& getSelector() const;
    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
