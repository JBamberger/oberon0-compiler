#pragma once
#include "Node.h"
#include "DeclarationsNode.h"
#include "StatementSequenceNode.h"

class ProcedureDeclarationNode : public Node {
    std::string name_;
    std::unique_ptr<const FormalParameterList> params_;
    std::unique_ptr<const DeclarationsNode> declarations_;
    std::unique_ptr<const StatementSequenceNode> statements_;

  public:
    ProcedureDeclarationNode(const FilePos& pos,
                             std::string name,
                             const FormalParameterList * params,
                             const DeclarationsNode* declarations,
                             const StatementSequenceNode* statements);
    ~ProcedureDeclarationNode() override;


    const std::string& getName() const;
    const std::unique_ptr<const FormalParameterList>& getParams() const;
    const std::unique_ptr<const DeclarationsNode>& getDeclarations() const;
    const std::unique_ptr<const StatementSequenceNode>& getStatements() const;
    void visit(NodeVisitor* visitor) const override;

    void print(std::ostream& stream) const override;
};
