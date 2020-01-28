#pragma once
#include "FieldDeclarationNode.h"
#include "MemberLayout.h"
#include "Scope.h"
#include "TypeNode.h"
#include <vector>

class RecordTypeNode : public TypeNode {
  private:
    std::shared_ptr<Scope> scope_;
    MemberLayout<FieldDeclarationNode> members_;

  public:
    RecordTypeNode(std::shared_ptr<Scope> parent);
    ~RecordTypeNode() override;

    MemberLayout<FieldDeclarationNode>& getMembers();

    const MemberLayout<FieldDeclarationNode>& getMembers() const;

    const std::shared_ptr<Scope>& getScope() const;

    std::string getId() const override;

    size_t getByteSize() const override;

    void visit(NodeVisitor* visitor) const override;

    void print(std::ostream& stream) const override;
};
