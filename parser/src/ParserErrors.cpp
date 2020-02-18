/**
 * Copyright 2020 Jannik Bamberger
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ParserErrors.h"

std::unordered_map<error_id, std::string> error_messages = {
    {error_id::E000, "Syntax error: Expected {:s} but got '{:s}'."},
    {error_id::E001, "Duplicate identifier '{:s}' in scope."},
    {error_id::E002, "Constant declaration does not evaluate to constant value."},
    {error_id::E003, "Array size does not evaluate to a constant value."},
    {error_id::E004, "Array size must be greater than '0' but is '{:d}'."},
    {error_id::E005, "'{:s}' is not a type."},
    {error_id::E006, ""},
    {error_id::E007, ""},
    {error_id::E008, ""},
    {error_id::E009, ""},
    {error_id::E010, "ARRAY formal parameter is marked as VAR"},
    {error_id::E011, "RECORD formal parameter is marked as VAR"},
    {error_id::E012, "Arithmetic operators operate only on 'INTEGER' but got '{:s}'."},
    {error_id::E013,
     "Comparison operators require the same type for both args, got '{:s}' and '{:s}'."},
    {error_id::E014, "Boolean operators operate only on 'BOOLEAN' but got '{:s}'."},
    {error_id::E015, "Undeclared identifier '{:s}'."},
    {error_id::E016, "Array selector can only be used on ARRAY types but the type is '{:s}'."},
    {error_id::E017, "Index {:d} out of bounds [0,{:d}]."},
    {error_id::E018, "Field selector can only be used on RECORD types but the type is '{:s}'."},
    {error_id::E019, "Field '{:s}' does not exist in record '{:s}'."},
    {error_id::E020, "Number of actual parameters ({:d}) and formal parameters ({:d}) of function "
                     "'{:s}' must match."},
    {error_id::E021, "Parameter Type mismatch. Expected '{:s}' but got '{:s}'."},
    {error_id::E022,
     "Parameter '{:s}' is passed by reference but the actual value is not assignable."},
    {error_id::E023, "Reference to invalid identifier type. id = '{:s}'."},
    {error_id::E024, "The assignment lhs must be assignable."},
    {error_id::E025, "The types of rhs and lhs of an assignment must match but are {:s} and {:s}."},
    {error_id::E026, "The type of IF-conditions must be 'BOOLEAN' but is '{:s}'."},
    {error_id::E027, "The type of WHILE-conditions must be 'BOOLEAN' but is '{:s}'."},
    {error_id::E028, "Expected equal MODULE names but got '{:s}' and '{:s}'."},
    {error_id::E029, "Expected equal PROCEDURE names but got '{:s}' and '{:s}'."},
    {error_id::E030, "Array indices must be of type 'INTEGER' but is '{:s}'."},
    {error_id::E031, "Identifier does not reference a procedure. id = '{:s}'."}};
