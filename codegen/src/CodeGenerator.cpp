/**
 * Copyright 2020 Jannik Bamberger
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
