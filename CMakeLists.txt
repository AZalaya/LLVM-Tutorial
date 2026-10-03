cmake_minimum_required(VERSION 3.23)

project(LLVM-Tutorial)

add_executable(CKal)

target_compile_features(CKal PRIVATE cxx_std_20)

option(DEBUG "Enable debug output" OFF)
option(VERBOSE "Enable verbose output" OFF)

if (DEBUG OR VERBOSE)
    add_definitions(-DDEBUG)
endif()

target_include_directories(CKal PUBLIC "include")
target_include_directories(CKal PUBLIC "include/front_end")
target_include_directories(CKal PUBLIC "include/utils")

add_subdirectory("include")
add_subdirectory("src")