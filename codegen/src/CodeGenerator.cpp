#include "CodeGenerator.h"
#include <X86_64CodeGenerator.h>

std::unique_ptr<CodeGenerator> CodeGenerator::create(Architecture arch)
{
    switch (arch) {
    case Architecture::X86_64:
        return std::make_unique<X86_64CodeGenerator>();
    default:
        throw std::runtime_error("The provided architecture is invalid.");
    }
}
