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
        if (!list_.empty()) {
            stream << *list_.at(0);
            for (auto i = 1; i < list_.size(); ++i) { stream << ", " << *(list_.at(i)); }
        }
        stream << ")";
    }
};
