
add_subdirectory(ast)

set(SOURCES
    "include/Parser.h"       "src/Parser.cpp"
    "include/PrintVisitor.h" "src/PrintVisitor.cpp"
    "include/ParserErrors.h" "src/ParserErrors.cpp"
)


add_library(oberon0c_parser ${SOURCES})
target_include_directories(oberon0c_parser PUBLIC
    "include"
    oberon0c_scanner
    oberon0c_parser_ast
    fmt::fmt
)
target_link_libraries(oberon0c_parser PUBLIC
    oberon0c_scanner
    oberon0c_parser_ast
    fmt::fmt
)