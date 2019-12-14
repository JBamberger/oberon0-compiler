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

    const std::string& getName() const;
    const std::unique_ptr<const DeclarationsNode>& getDeclarations() const;
    const std::unique_ptr<const StatementSequenceNode>& getStatements() const;
    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
