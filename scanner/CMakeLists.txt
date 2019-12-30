
set(SOURCES
    "include/IdentToken.h"  "src/IdentToken.cpp"
    "include/NumberToken.h" "src/NumberToken.cpp"
    "include/Scanner.h"     "src/Scanner.cpp"
    "include/StringToken.h" "src/StringToken.cpp"
    "include/Token.h"       "src/Token.cpp"
)

add_library(oberon0c_scanner ${SOURCES})
target_include_directories(oberon0c_scanner PUBLIC "include" oberon0c_util)
target_link_libraries(oberon0c_scanner PUBLIC oberon0c_util)