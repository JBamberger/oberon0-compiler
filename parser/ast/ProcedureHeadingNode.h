#pragma once
#include "DeclarationListNode.h"
#include "Node.h"

class ProcedureHeadingNode : public Node {
    std::string name_;
    std::unique_ptr<const FormalParameterList> params_;

  public:
    ProcedureHeadingNode(const FilePos& pos, std::string name, const FormalParameterList* params);
    ~ProcedureHeadingNode() override;

    void print(std::ostream& stream) const override;
};