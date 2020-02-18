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

#include "Scope.h"
#include "BasicTypeNode.h"
#include <cassert>
#include <utility>

Scope::Scope(std::string name) : name_(std::move(name))
{
}

Scope::Scope(std::string name, std::shared_ptr<Scope> parent)
    : parent_(std::move(parent)), name_(std::move(name))
{
    parent_->children_.push_back(this);
}

bool Scope::declareIdentifier(std::string name, Node* value)
{
    return declareIdentifier(std::make_unique<Symbol>(std::move(name), value));
}

bool Scope::declareIdentifier(std::unique_ptr<Symbol> symbol)
{
    const auto local = resolveIdentifierLocally(symbol->identifier);

    if (local != nullptr) {
        return false;
    }
    identifier_map_.insert(std::map<std::string, std::unique_ptr<Symbol>>::value_type(
        symbol->identifier, std::move(symbol)));
    return true;
}

Symbol* Scope::resolveIdentifier(const std::string& identifier) const
{
    for (auto next = this; next != nullptr; next = next->parent_.get()) {
        const auto pair = next->identifier_map_.find(identifier);
        if (pair != next->identifier_map_.end()) {
            return pair->second.get();
        }
    }
    return nullptr;
}

Symbol* Scope::resolveIdentifierLocally(const std::string& identifier) const
{
    const auto pair = identifier_map_.find(identifier);
    if (pair != identifier_map_.end()) {
        return pair->second.get();
    }
    return nullptr;
}

const std::shared_ptr<Scope>& Scope::getParent() const
{
    return parent_;
}

void Scope::declareDefaultType(const std::shared_ptr<BasicTypeNode>& type)
{
    const auto result = declareIdentifier(std::make_unique<Symbol>(type->getName(), type.get()));
    assert(result);
}

void Scope::print(const int indent, std::ostream& out) const
{
    for (auto i = 0; i < indent - 1; ++i) {
        out << "|   ";
    }
    if (indent >= 1) {
        out << "+---";
    }
    out << "Scope " << name_ << "\n";

    for (const auto& pair : identifier_map_) {
        for (auto i = 0; i < indent + 1; ++i) {
            out << "|   ";
        }
        out << pair.first << "\n";
    }

    for (auto i = 0; i < indent + 1; ++i) {
        out << "|   ";
    }
    out << "\n";

    for (auto child : children_) {
        child->print(indent + 1, out);
        for (auto i = 0; i < indent + 1; ++i) {
            out << "|   ";
        }
        out << "\n";
    }
}
