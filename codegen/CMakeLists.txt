set(SOURCES
        include/Architecture.h
        "include/CodeGenerator.h"
        src/CodeGenerator.cpp
        include/X86_64CodeGenerator.h
        src/X86_64CodeGenerator.cpp
        )


add_library(oberon0c_codegen ${SOURCES})
target_include_directories(oberon0c_codegen PUBLIC
        "include"
        oberon0c_ast
        fmt::fmt
        )
target_link_libraries(oberon0c_codegen PUBLIC
        oberon0c_ast
        fmt::fmt
        )