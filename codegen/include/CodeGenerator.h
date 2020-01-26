#pragma once

#include <memory>
#include <ModuleNode.h>
#include "Architecture.h"

class CodeGenerator {

public:
    virtual void generate(std::unique_ptr<ModuleNode> ast, std::ostream* output) = 0;

    static std::unique_ptr<CodeGenerator> create(Architecture arch);
};