#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "ExpressionNode.h"
#include "TypeNode.h"

// resolve type names to type references

// struct Type {
//    // basic, array, record, function, pointer
//};
//
// struct Constant {
//    // string, number, boolean
//    bool isString();
//    bool isNumber();
//};

struct Symbol {
};

class Scope {
    std::shared_ptr<Scope> parent_;
    std::map<std::string, std::unique_ptr<ConstantNode>> constants_;
    std::vector<std::unique_ptr<TypeNode>> types_;
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

    /**
     * \brief Inserts a new constant into the scope.
     *
     * TODO: handle duplicate types
     *
     * \param identifier constant name
     */
    void declareConstant(const std::string& identifier, std::unique_ptr<ConstantNode> constant);

    /**
     * \brief Inserts a new type into the scope.
     *
     * TODO: handle duplicate types
     *
     * \return the IR type representation
     */
    TypeNode* declareType(std::unique_ptr<TypeNode> type);

    /**
     * \brief Resolves the type reference and returns the type in the intermediate representation.
     * \return IR type or null if the type could not be resolved.
     */
    TypeNode* resolveType(/* type reference */);

    /**
     * \brief Inserts a new identifier into the scope.
     *
     * TODO: handle duplicate identifiers
     *
     * \param identifier the new identifier
     * \param symbol description of the identifier
     */
    void declareIdentifier(const std::string& identifier, std::unique_ptr<Symbol> symbol);

    /**
     * \brief Searches the identifiers type and returns its type.
     * The function honors the scoping rules and will search in the parent scopes if necessary.
     *
     * \param identifier name of the object
     *
     * \return The descriptor info of the identifier or null if the identifier is undeclared.
     */
    Symbol* resolveIdentifier(const std::string& identifier);

    const std::shared_ptr<Scope>& getParent() const;
};
