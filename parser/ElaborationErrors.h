#pragma once
#include "fmt/format.h"

// T001: Constant expressions must be evaluated
// T002: Arithmetic operators produce INTEGER
// T003: Comparison operators produce BOOLEAN
// T004: Boolean operators produce BOOLEAN
// T005: RECORD types are passed by reference
// T006: ARRAY types are passed by reference

// E001: Names must be unique in the scope
// E002: Constant declarations must evaluate to constants
// E003: Array sizes must evaluate to constants
// E004: Array sizes must be greater than 0
// E005: Array type must exist
// E006: Record field names must be unique in the record
// E007: Record field types must exist
// E008: Variable types must exist
// E009: Procedure identifiers must be unique in the procedure
// E010: ARRAY formal parameters cannot be VAR
// E011: RECORD formal parameters cannot be VAR
// E012: Arithmetic operators operate only on INTEGER
// E013: Comparison operators require the same type for both arguments
// E014: Boolean operators operate only on BOOLEAN
// E015: Names must be declared before they are used
// E016: Array selectors can only be used on ARRAY types
// E017: Constant array indices must be within the array bounds
// E018: Field selectors can only be used on RECORD types
// E019: Field selector fields must exist on the given RECORD type
// E020: Actual and formal parameter counts must match for procedure calls
// E021: Actual and formal parameter types must match for procedure calls
// E022: Constants cannot be used for VAR parameters
// E023: Expressions cannot be used for VAR parameters
// E024: The assignment lhs must be assignable
// E025: The types of rhs and lhs of an assignment must match
// E026: The type of IF-conditions must be BOOLEAN
// E027: The type of WHILE-conditions must be BOOLEAN
// E028: Module names at begin and end must match
// E029: Procedure names at begin and end must match
// E030: Array indices must be of type INTEGER
// E031: Procedure call name must refer to a procedure declaration

// W001: WHILE-condition is constant
// W002: IF-condition is constant


inline auto errorDuplicateIdentifier(const std::string& identifier)
{
    return format(FMT_STRING("Duplicate identifier '{:s}'."), identifier);
}

inline auto errorExpressionNotConst() { return "Expression must be constant."; }

inline auto errorSizeLtZero(int size)
{
    return format(FMT_STRING("Array size must be > 0 but is {:d}"), size);
}

inline auto errorMissingDeclaration(const std::string& identifier)
{
    return format(FMT_STRING("Missing declaration for '{:s}'."), identifier);
}

inline auto errorStructuredTypeIsVar(const std::string& identifier)
{
    return format(FMT_STRING("Structured variable '{:s}' is declared as VAR."), identifier);
}

inline auto errorTypeError(const std::string& expected_type, const std::string& actual_type)
{
    return format(FMT_STRING("Type error. Expected '{:s}', got '{:s}'."), expected_type,
                  actual_type);
}

inline auto errorArraySelectorOnNonArray() { return "Array selector on non-array."; }

inline auto errorMemberSelectorOnNonRecord() { return "Member selector on non-record."; }

inline auto errorInvalidMember(const std::string& member_name, const std::string& record_name)
{
    return format(FMT_STRING("Invalid member name '{:s}' for record '{:s}'."), member_name,
                  record_name);
}

inline auto errorIndexOutOfRange(int index, int array_size)
{
    return format(FMT_STRING("Array index {:d} out of range [0..{:d}]."), index, array_size);
}

inline auto errorParamMismatch() { return "Actual and formal parameters do not match."; }

inline auto errorIllegalVarArgument() { return "Illegal use of VAR on non-variable."; }

inline auto errorLhsNotAssignable() { return "Left-hand side is not assignable"; }