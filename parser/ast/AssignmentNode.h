#pragma once
#include "MemberReferenceNode.h"
#include "StatementNode.h"

class AssignmentNode : public StatementNode {
    std::unique_ptr<const MemberReferenceNode> assignee_;
    std::unique_ptr<const ExpressionNode> value_;

  public:
    AssignmentNode(const FilePos& pos,
                   const MemberReferenceNode* assignee,
                   const ExpressionNode* value);
    virtual ~AssignmentNode() override;
    void print(std::ostream& stream) const override;
};
