#pragma once

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "BasicTypeNode.h"
#include "ExpressionNode.h"
#include "TypeNode.h"
#include <ostream>

// resolve type names to type references

struct Symbol {
    std::string identifier;
    Node* value;

    Symbol(std::string identifier, Node* value) : identifier(std::move(identifier)), value(value) {}
};

class Scope {
    std::shared_ptr<Scope> parent_;
    std::vector<Scope*> children_;

    std::vector<std::unique_ptr<BasicTypeNode>> default_types_;
    std::map<std::string, std::unique_ptr<Symbol>> identifier_map_;

  public:
    /**
     * \brief Creates a top-level scope initialized with all predefined types
     */
    explicit Scope();

    /**
     * \brief Creates a nested scope with the given parent. Name resolution will consider the
     * parent. \param parent the parent scope
     */
    explicit Scope(std::shared_ptr<Scope> parent);

    [[nodiscard]] bool declareIdentifier(std::string name, Node* value);
    /**
     * \brief Inserts a new identifier into the scope.
     *
     * TODO: handle duplicate identifiers
     *
     * \param symbol description of the identifier
     * \return true if the identifier was added successfully
     */
    [[nodiscard]] bool declareIdentifier(std::unique_ptr<Symbol> symbol);

    /**
     * \brief Searches the identifiers type and returns its type.
     * The function honors the scoping rules and will search in the parent scopes if necessary.
     *
     * \param identifier name of the object
     *
     * \return The descriptor info of the identifier or null if the identifier is undeclared.
     */
    Symbol* resolveIdentifier(const std::string& identifier);

    Symbol* resolveIdentifierLocally(const std::string& identifier);

    const std::shared_ptr<Scope>& getParent() const;

    friend std::ostream& operator<<(std::ostream& os, const Scope& obj)
    {
        obj.print(0, os);
        return os;
    }

  private:
    void declareDefaultType(const std::string& name);
    void print(int indent, std::ostream& out) const;
};
