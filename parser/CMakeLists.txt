#
# Copyright 2020 Jannik Bamberger
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#


set(SOURCES
    "include/ParseException.h"
    "include/Parser.h"       "src/Parser.cpp"
    "include/PrintVisitor.h" "src/PrintVisitor.cpp"
    "include/ParserErrors.h" "src/ParserErrors.cpp"
)


add_library(oberon0c_parser ${SOURCES})
target_include_directories(oberon0c_parser PUBLIC
    "include"
    oberon0c_scanner
    oberon0c_ast
    fmt::fmt
)
target_link_libraries(oberon0c_parser PUBLIC
    oberon0c_scanner
    oberon0c_ast
    fmt::fmt
)

add_subdirectory("test")
