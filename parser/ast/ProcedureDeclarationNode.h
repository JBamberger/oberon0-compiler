#pragma once
#include "Node.h"
#include "ProcedureBodyNode.h"
#include "ProcedureHeadingNode.h"

class ProcedureDeclarationNode : public Node {
    std::unique_ptr<const ProcedureHeadingNode> heading_;
    std::unique_ptr<const ProcedureBodyNode> body_;

  public:
    ProcedureDeclarationNode(const FilePos& pos,
                             const ProcedureHeadingNode* heading,
                             const ProcedureBodyNode* body);
    ~ProcedureDeclarationNode() override;

    const std::unique_ptr<const ProcedureHeadingNode>& getHeading() const;
    const std::unique_ptr<const ProcedureBodyNode>& getBody() const;
    void print(std::ostream& stream) const override;
};