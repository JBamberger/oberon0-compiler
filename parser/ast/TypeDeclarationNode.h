#pragma once
#include "Node.h"
#include "TypeNode.h"

class TypeDeclarationNode : public Node {
    std::string name_;
    std::shared_ptr<TypeNode> type_;

  public:
    TypeDeclarationNode(const FilePos& pos, std::string name, std::shared_ptr<TypeNode> type);
    ~TypeDeclarationNode() override;

    const std::string& getName() const;
    const std::shared_ptr<TypeNode>& getType() const;
    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
