cmake_minimum_required(VERSION 3.23)

project(LLVM-Tutorial)

add_executable(CKal)

target_sources(CKal
    PRIVATE
    src/main.cpp
    src/front_end/lexer.cpp
    src/front_end/tokens.cpp
    src/front_end/AST.cpp
    
    PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
    include
    FILES
    include/front_end/lexer.hpp
    include/front_end/tokens.hpp
    include/front_end/AST.hpp
)
