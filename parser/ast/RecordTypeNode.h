#pragma once
#include "FieldDeclarationNode.h"
#include "Scope.h"
#include "TypeNode.h"
#include <vector>

class RecordTypeNode : public TypeNode {
    std::shared_ptr<Scope> scope_;
    std::unique_ptr<std::vector<std::unique_ptr<FieldDeclarationNode>>> members_;

  public:
    RecordTypeNode(const FilePos& pos, std::shared_ptr<Scope> parent);
    ~RecordTypeNode() override;

    std::vector<std::unique_ptr<FieldDeclarationNode>>* getMembers() const;

    const std::shared_ptr<Scope>& getScope() const;

    std::string getId() const override;

    void visit(NodeVisitor* visitor) const override;

    void print(std::ostream& stream) const override;
};
