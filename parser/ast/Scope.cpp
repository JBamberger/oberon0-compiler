#include "Scope.h"
#include "BasicTypeNode.h"
#include <utility>

Scope::Scope()
{
    // Declare builtin types in the hightest level scope
    declareType(std::make_unique<BasicTypeNode>(FilePos(), "INTEGER"));
}

Scope::Scope(std::shared_ptr<Scope> parent) : parent_(std::move(parent)) {}

void Scope::declareConstant(const std::string& identifier, std::unique_ptr<ConstantNode> constant)
{
    const auto c = constants_.find(identifier);
    if (c == constants_.end()) {
        constants_.insert(
            std::pair<std::string, std::unique_ptr<ConstantNode>>(identifier, std::move(constant)));
    } else {
        // TODO: constant name already present, handle correctly
        std::cout << "Duplicate constant" << std::endl;
        std::terminate();
    }
}

TypeNode* Scope::declareType(std::unique_ptr<TypeNode> type)
{
    types_.push_back(std::move(type));
    return types_.at(types_.size() - 1).get();
}

TypeNode* Scope::resolveType()
{
    // TODO: implement
    return nullptr;
}

void Scope::declareIdentifier(const std::string& identifier, std::unique_ptr<Symbol> symbol)
{
    const auto id = identifier_map_.find(identifier);
    if (id == identifier_map_.end()) {
        identifier_map_.insert(
            std::pair<std::string, std::unique_ptr<Symbol>>(identifier, std::move(symbol)));
    } else {
        // TODO: identifier name already present, handle correctly
        std::cout << "Duplicate identifier" << std::endl;
        std::terminate();
    }
}

Symbol* Scope::resolveIdentifier(const std::string& identifier)
{
    const auto pair = identifier_map_.find(identifier);
    if (pair != identifier_map_.end()) {
        return pair->second.get();
    }
    return nullptr;
}

const std::shared_ptr<Scope>& Scope::getParent() const { return parent_; }
