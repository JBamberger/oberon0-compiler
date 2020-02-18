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

cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(oberon0c VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)


# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory("3rdparty/googletest")
add_subdirectory("3rdparty/fmt")

if(MSVC)
  add_compile_options(/W4) # /WX
else()
  add_compile_options(-Wall -Wextra -pedantic) # -Werror
endif()

add_subdirectory("util")
add_subdirectory("scanner")
add_subdirectory("ast")
add_subdirectory("parser")
add_subdirectory("codegen")

set(SOURCES
    "main.cpp"
)

add_executable(            oberon0c ${SOURCES})
target_include_directories(oberon0c PRIVATE oberon0c_parser oberon0c_codegen)
target_link_libraries(     oberon0c PRIVATE oberon0c_parser oberon0c_codegen)



