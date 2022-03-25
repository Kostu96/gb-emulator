add_subdirectory(cpp-common)

set_target_properties(
    cpp-common-lib
    PROPERTIES
    FOLDER external
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

message(STATUS "Configuring googletest...")
execute_process(COMMAND ${CMAKE_COMMAND}
	-S ${CMAKE_CURRENT_SOURCE_DIR}/gtest
	-B ${CMAKE_CURRENT_BINARY_DIR}/build_gtest
	-D BUILD_GMOCK=0
	-D INSTALL_GTEST=0
)

message(STATUS "Building googletest Debug...")
execute_process(COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/build_gtest --config Debug)
message(STATUS "Building googletest Release...")
execute_process(COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/build_gtest --config Release)

set(GTEST_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/gtest/googletest/include PARENT_SCOPE)
set(GTEST_LIB_DIR ${CMAKE_CURRENT_BINARY_DIR}/build_gtest/lib PARENT_SCOPE)

message(STATUS "Configuring SFML...")
execute_process(COMMAND ${CMAKE_COMMAND}
	-S ${CMAKE_CURRENT_SOURCE_DIR}/SFML
	-B ${CMAKE_CURRENT_BINARY_DIR}/build_SFML
	-D SFML_BUILD_NETWORK=0
	-D SFML_GENERATE_PDB=0
	-D BUILD_SHARED_LIBS=0
	-D SFML_USE_STATIC_STD_LIBS=1
)

message(STATUS "Building SFML Debug...")
execute_process(COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/build_SFML --config Debug)
message(STATUS "Building SFML Release...")
execute_process(COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/build_SFML --config Release)

set(SFML_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SFML/include PARENT_SCOPE)
set(SFML_LIB_DIR ${CMAKE_CURRENT_BINARY_DIR}/build_SFML/lib PARENT_SCOPE)
