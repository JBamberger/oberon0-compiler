#pragma once
#include "DeclarationListNode.h"
#include "Node.h"

class ProcedureHeadingNode : public Node {
    std::string name_;
    std::unique_ptr<const FormalParameterList> params_;

  public:
    ProcedureHeadingNode(const FilePos& pos, std::string name, const FormalParameterList* params);
    ~ProcedureHeadingNode() override;

    const std::string& getName() const;
    const std::unique_ptr<const FormalParameterList>& getParams() const;
    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
