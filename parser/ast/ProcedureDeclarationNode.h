#pragma once
#include "Node.h"
#include "ProcedureHeadingNode.h"
#include "ProcedureBodyNode.h"

class ProcedureDeclarationNode : public Node {
    std::unique_ptr<const ProcedureHeadingNode> heading_;
    std::unique_ptr<const ProcedureBodyNode> body_;

  public:
    ProcedureDeclarationNode(const FilePos& pos,
                             const ProcedureHeadingNode* heading,
                             const ProcedureBodyNode* body);
    ~ProcedureDeclarationNode() override;

    void print(std::ostream& stream) const override;
};