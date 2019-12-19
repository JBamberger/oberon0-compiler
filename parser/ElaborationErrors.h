#pragma once
#include "fmt/format.h"

inline auto errorDuplicateIdentifier(const std::string& identifier)
{
    return format(FMT_STRING("Duplicate identifier '{:s}'."), identifier);
}

inline auto errorExpressionNotConst(const std::string& identifier)
{
    return format(FMT_STRING("Expression must be constant '{:s}'"), identifier);
}

inline auto errorSizeLtZero(int size)
{
    return format(FMT_STRING("Array size must be >= 0 but is {:d}"), size);
}

inline auto errorMissingTypeDecl(const std::string& identifier)
{
    return format(FMT_STRING("Missing type declaration for '{:s}'."), identifier);
}

inline auto errorStructuredTypeIsVar(const std::string& identifier)
{
    return format(FMT_STRING("Structured variable '{:s}' is declared as VAR."), identifier);
}

inline auto errorTypeError(const std::string& expected_type, const std::string& actual_type)
{
    return format(FMT_STRING("Type error. Expected '{:s}', got '{:s}'."), expected_type, actual_type);
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