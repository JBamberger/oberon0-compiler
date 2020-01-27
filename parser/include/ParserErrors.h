#pragma once
#include "fmt/format.h"
#include <unordered_map>

enum class error_id : char {
    E000, // * Syntax error
    E001, // * Names must be unique in the scope
    E002, // * Constant declarations must evaluate to constants
    E003, // * Array sizes must evaluate to constants
    E004, // * Array sizes must be greater than 0
    E005, // * Types must exist
    E006, //   unused
    E007, //   unused
    E008, //   unused
    E009, //   unused
    E010, // * ARRAY formal parameters cannot be VAR
    E011, // * RECORD formal parameters cannot be VAR
    E012, // * Arithmetic operators operate only on INTEGER
    E013, // * Comparison operators require the same type for both arguments
    E014, // * Boolean operators operate only on BOOLEAN
    E015, // * Names must be declared before they are used
    E016, // * Array selectors can only be used on ARRAY types
    E017, // * Constant array indices must be within the array bounds
    E018, // * Field selectors can only be used on RECORD types
    E019, // * Field selector fields must exist on the given RECORD type
    E020, // * Parameter counts must match for procedure calls
    E021, // * Actual and formal parameter types must match for procedure calls
    E022, // * VAR parameters must be assignable
    E023, // * Allow references to constants, parameters and variables
    E024, // * The assignment lhs must be assignable
    E025, // * The types of rhs and lhs of an assignment must match
    E026, // * The type of IF-conditions must be BOOLEAN
    E027, // * The type of WHILE-conditions must be BOOLEAN
    E028, // * Module names at begin and end must match
    E029, // * Procedure names at begin and end must match
    E030, // * Array indices must be of type INTEGER
    E031  // * Procedure call name must refer to a procedure declaration
};

extern std::unordered_map<error_id, std::string> error_messages;

template <typename... Types>
std::string getErrMsg(const error_id id, Types&&... args)
{
    return fmt::format(error_messages.at(id), std::forward<Types>(args)...);
}

// T001: Constant expressions must be evaluated
// T002: Arithmetic operators produce INTEGER
// T003: Comparison operators produce BOOLEAN
// T004: Boolean operators produce BOOLEAN
// T005: RECORD types are passed by reference
// T006: ARRAY types are passed by reference

// W001: WHILE-condition is constant
// W002: IF-condition is constant

// Get-Content .\parser\src\Parser.cpp | Select-String 'E\d{3}'  | %{$_.ToString().Trim()} |
// Sort-Object  | Get-Unique