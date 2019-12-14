#pragma once
#include "Node.h"
#include "TypeNode.h"

class TypeDeclarationNode : public Node {
    std::string name_;
    std::shared_ptr<const TypeNode> type_;

  public:
    TypeDeclarationNode(const FilePos& pos, std::string name, const TypeNode* type);
    ~TypeDeclarationNode() override;

    const std::string& getName() const;
    const std::shared_ptr<const TypeNode>& getType() const;
    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
