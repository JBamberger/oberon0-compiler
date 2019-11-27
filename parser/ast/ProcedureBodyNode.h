#pragma once
#include "DeclarationsNode.h"
#include "Node.h"
#include "StatementSequenceNode.h"

class ProcedureBodyNode : public Node {
    std::string name_;
    std::unique_ptr<const DeclarationsNode> declarations_;
    std::unique_ptr<const StatementSequenceNode> statements_;

  public:
    ProcedureBodyNode(const FilePos& pos,
                      std::string name,
                      const DeclarationsNode* declarations,
                      const StatementSequenceNode* statements);
    ~ProcedureBodyNode() override;

    void print(std::ostream& stream) const override;
};
