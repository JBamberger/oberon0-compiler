#pragma once
#include "FieldDeclarationNode.h"
#include "Scope.h"
#include "TypeNode.h"
#include "TypedIdentifierListNode.h"
#include <vector>

class RecordTypeNode : public TypeNode {
    std::shared_ptr<Scope> scope_;
    std::vector<std::shared_ptr<const TypeNode>> types_;
    std::vector<std::unique_ptr<const FieldDeclarationNode>> members_;

  public:
    RecordTypeNode(const FilePos& pos, std::shared_ptr<Scope> parent);
    ~RecordTypeNode() override;
    void addFields(const FieldListNode* fields);

    const std::vector<std::shared_ptr<const TypeNode>>& getTypes() const;
    const std::vector<std::unique_ptr<const FieldDeclarationNode>>& getMembers() const;
    const std::shared_ptr<Scope>& getScope() const;

    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
