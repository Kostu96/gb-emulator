add_subdirectory(cpp-common)

set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
add_subdirectory(gtest)
    
set_target_properties(
    cpp-common-lib
    gtest
    gtest_main
    PROPERTIES
    FOLDER third_party
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_GENERATE_PDB OFF CACHE BOOL "" FORCE)
add_subdirectory(SFML)

set_target_properties(
    sfml-audio
    sfml-graphics
    sfml-main
    sfml-system
    sfml-window
    PROPERTIES
    FOLDER third_party/SFML
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)
