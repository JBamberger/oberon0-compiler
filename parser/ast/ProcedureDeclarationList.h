#pragma once
#include <vector>
#include "ProcedureDeclarationNode.h"

class ProcedureDeclarationList {
    std::vector<std::unique_ptr<const ProcedureDeclarationNode>> list_;

  public:
    ProcedureDeclarationList(const FilePos& pos,
                             std::vector<std::unique_ptr<const ProcedureDeclarationNode>> list)
        : list_(std::move(list))
    {
    }
};
