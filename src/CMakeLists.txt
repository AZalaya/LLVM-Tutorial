target_sources(CKal
    PRIVATE
    main.cpp
)

add_subdirectory("front_end")
add_subdirectory("llvm")
add_subdirectory("utils")