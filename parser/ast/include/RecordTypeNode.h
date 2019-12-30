#pragma once
#include "FieldDeclarationNode.h"
#include "Scope.h"
#include "TypeNode.h"
#include <vector>

class RecordTypeNode : public TypeNode {
  public:
    using FieldDeclList = std::vector<std::unique_ptr<FieldDeclarationNode>>;

  private:
    std::shared_ptr<Scope> scope_;
    FieldDeclList members_;

  public:
    RecordTypeNode(const FilePos& pos, std::shared_ptr<Scope> parent);
    ~RecordTypeNode() override;

    FieldDeclList& getMembers();

    const FieldDeclList& getMembers() const;

    const std::shared_ptr<Scope>& getScope() const;

    std::string getId() const override;

    void visit(NodeVisitor* visitor) const override;

    void print(std::ostream& stream) const override;
};
