#include "Scope.h"
#include "BasicTypeNode.h"
#include <cassert>
#include <utility>

std::shared_ptr<BasicTypeNode> Scope::INTEGER =
    std::make_shared<BasicTypeNode>(FilePos(), "INTEGER");
std::shared_ptr<BasicTypeNode> Scope::BOOLEAN =
    std::make_shared<BasicTypeNode>(FilePos(), "BOOLEAN");
std::shared_ptr<BasicTypeNode> Scope::STRING = std::make_shared<BasicTypeNode>(FilePos(), "STRING");

Scope::Scope(std::string name) : name_(std::move(name))
{
    // Declare builtin types in the hightest level scope
    declareDefaultType(INTEGER);
    declareDefaultType(BOOLEAN);
    declareDefaultType(STRING);
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

Symbol* Scope::resolveIdentifier(const std::string& identifier)
{
    for (auto next = this; next != nullptr; next = next->parent_.get()) {
        const auto pair = next->identifier_map_.find(identifier);
        if (pair != next->identifier_map_.end()) {
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
