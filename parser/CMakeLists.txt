
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