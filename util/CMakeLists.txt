
set(SOURCES
    "include/Logger.h" "src/Logger.cpp"
        include/FilePos.h)

add_library(oberon0c_util ${SOURCES})
target_include_directories(oberon0c_util PUBLIC "include")