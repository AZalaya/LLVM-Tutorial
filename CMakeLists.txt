cmake_minimum_required(VERSION 3.23)

project(LLVM-Tutorial)

add_executable(CKal)

target_compile_features(CKal PRIVATE cxx_std_20)

option(DEBUG "Enable debug output" ON)
option(OPTIMIZE "Enable LLVM IR optimization" OFF)

if (DEBUG)
    add_definitions(-DDEBUG)
endif()

if (OPTIMIZE)
    add_definitions(-DOPTIMIZE)
endif()

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

if(${LLVM_LINK_LLVM_DYLIB})
    message(STATUS "Linking to LLVM dynamic shared library object")
    set(llvm_libs LLVM)
else()
    message(STATUS "Linking to separate LLVM static libraries")
    llvm_map_components_to_libnames(llvm_libs support core irreader)
endif()

target_link_libraries(CKal ${llvm_libs})

target_include_directories(CKal PUBLIC "include")
target_include_directories(CKal PUBLIC "include/front_end")
target_include_directories(CKal PUBLIC "include/llvm")
target_include_directories(CKal PUBLIC "include/utils")

add_subdirectory("include")
add_subdirectory("src")