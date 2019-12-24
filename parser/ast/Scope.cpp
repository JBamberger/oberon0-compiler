#include "Scope.h"
#include "BasicTypeNode.h"
#include <cassert>
#include <utility>

Scope::Scope()
{
    // Declare builtin types in the hightest level scope
    declareDefaultType("INTEGER");
    declareDefaultType("BOOLEAN");
}

Scope::Scope(std::shared_ptr<Scope> parent) : parent_(std::move(parent)) {}

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

Symbol* Scope::resolveIdentifier(const std::string& identifier)
{
    for (auto next = this; next != nullptr; next = next->parent_.get()) {
        const auto pair = identifier_map_.find(identifier);
        if (pair != identifier_map_.end()) {
            return pair->second.get();
        }
    }
    return nullptr;
}

Symbol* Scope::resolveIdentifierLocally(const std::string& identifier)
{
    const auto pair = identifier_map_.find(identifier);
    if (pair != identifier_map_.end()) {
        return pair->second.get();
    }
    return nullptr;
}

const std::shared_ptr<Scope>& Scope::getParent() const { return parent_; }

void Scope::declareDefaultType(const std::string& name)
{
    auto type = std::make_unique<BasicTypeNode>(FilePos(), name);
    const auto ptr = type.get();
    default_types_.push_back(std::move(type));
    const auto result = declareIdentifier(std::make_unique<Symbol>(name, ptr));
    assert(result);
}
