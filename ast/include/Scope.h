#pragma once

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "BasicTypeNode.h"
#include "ExpressionNode.h"
#include <ostream>

struct Symbol {
    std::string identifier;
    Node* value;

    Symbol(std::string identifier, Node* value) : identifier(std::move(identifier)), value(value) {}
};

class Scope {
    std::shared_ptr<Scope> parent_;
    std::vector<Scope*> children_;

    std::string name_;
    std::map<std::string, std::unique_ptr<Symbol>> identifier_map_;

  public:
    /**
     * \brief Creates a top-level scope initialized with all predefined types
     * \param name The name of the scope
     */
    explicit Scope(std::string name);

    /**
     * \brief Creates a nested scope with the given parent.
     * Name resolution will consider the parent.
     * \param parent the parent scope
     * \param name the name of the scope
     */
    explicit Scope(std::string name, std::shared_ptr<Scope> parent);

    /**
     * \brief Creates a new symbol and then inserts it into the scope.
     * \param name identifier of the symbol
     * \param value value of the symbol
     * \return true if the symbol was inserted correctly, false if it exists already
     */
    bool declareIdentifier(std::string name, Node* value);

    /**
     * \brief Inserts a new identifier into the scope.
     * \param symbol description of the identifier
     * \return true if the identifier was added successfully
     */
    bool declareIdentifier(std::unique_ptr<Symbol> symbol);

    /**
     * \brief Searches the identifier and returns the found symbol.
     * The function honors the scoping rules and will search in the parent scopes if necessary.
     * \param identifier name of the symbol
     * \return the found symbol of null if the symbol is not declared / does not exist
     */
    Symbol* resolveIdentifier(const std::string& identifier) const;

    /**
     * \brief Searches the identifier locally and returns the found symbol.
     * The search does not consider parent scopes. The identifier is only searched in this scope.
     * \param identifier name of the symbol
     * \return the found symbol or nullptr if it was not found
     */
    Symbol* resolveIdentifierLocally(const std::string& identifier) const;

    /**
     * \brief Returns the parent scope.
     * \return Pointer to the parent scope or null if this scope is the root scope
     */
    const std::shared_ptr<Scope>& getParent() const;

    friend std::ostream& operator<<(std::ostream& os, const Scope& obj)
    {
        obj.print(0, os);
        return os;
    }

  private:
    void declareDefaultType(const std::shared_ptr<BasicTypeNode>& type);

    void print(int indent, std::ostream& out) const;
};
