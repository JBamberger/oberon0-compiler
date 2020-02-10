#pragma once

#include "Architecture.h"
#include <ModuleNode.h>
#include <memory>

class CodeGenerator {

  public:
    virtual ~CodeGenerator() = default;
    virtual void generate(std::unique_ptr<ModuleNode> ast, std::ostream* output) = 0;

    static std::unique_ptr<CodeGenerator> create(Architecture arch);
};