#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

// resolve type names to type references

struct Type {
    // basic, array, record, function, pointer
};

struct Constant {
    // string, number, boolean
    bool isString();
    bool isNumber();
};

class Scope {
    std::shared_ptr<Scope> parent_;
    std::map<std::string, std::unique_ptr<Constant>> constants_;
    std::vector<std::unique_ptr<Type>> types_;
    std::map<std::string, Type*> identifier_map_;

  public:

    /**
     * \brief Inserts a new constant into the scope.
     *
     * TODO: handle duplicate types
     *
     * \param identifier constant name
     */
    void declareConstant(const std::string& identifier /*constant node*/);

    /**
     * \brief Inserts a new type into the scope.
     *
     * TODO: handle duplicate types
     *
     * \return the IR type representation
     */
    Type* declareType(/*node type*/);

    /**
     * \brief Resolves the type reference and returns the type in the intermediate representation.
     * \return IR type or null if the type could not be resolved.
     */
    Type* resolveType(/* type reference */);

    /**
     * \brief Inserts a new identifier into the scope.
     *
     * TODO: handle duplicate identifiers
     *
     * \param identifier the new identifier
     * \param type the identifiers type
     */
    void declareIdentifier(const std::string& identifier, Type* type);

    /**
     * \brief Searches the identifiers type and returns its type.
     * The function honors the scoping rules and will search in the parent scopes if necessary.
     *
     * \param identifier name of the object
     *
     * \return The type of the identifier or null if the identifier is undeclared.
     */
    Type* resolveIdentifier(const std::string& identifier);
};
