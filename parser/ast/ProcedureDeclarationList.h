#pragma once
#include "ProcedureDeclarationNode.h"
#include <vector>

class ProcedureDeclarationList : public Node {
    std::vector<std::unique_ptr<const ProcedureDeclarationNode>> list_;

  public:
    ProcedureDeclarationList(const FilePos& pos,
                             std::vector<std::unique_ptr<const ProcedureDeclarationNode>> list)
        : Node(NodeType::declaration_list, pos), list_(std::move(list))
    {
    }
    ~ProcedureDeclarationList() = default;

    void print(std::ostream& stream) const override
    {
        stream << "ProcedureDeclarationList(";
        for (const auto& v : list_) stream << *v << " ";
        stream << ")";
    }
};
