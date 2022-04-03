set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/blargg_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu_tests_8bit_moves.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu_tests_16bit_arithmetic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu_tests_16bit_moves.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu_tests_fixture.h
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu_tests_misc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gameboy_fixture.h
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mooneye_tests.cpp
)

add_executable(${TARGET_NAME_TESTS}
	${SOURCE_FILES}
)

target_include_directories(${TARGET_NAME_TESTS} PRIVATE
	${GTEST_INCLUDE_DIR}
)

target_compile_definitions(${TARGET_NAME_TESTS} PRIVATE
	GB_TESTS
)

target_link_libraries(${TARGET_NAME_TESTS}
    ${TARGET_NAME_LIB}
	${GTEST_LIB_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/gtest.lib
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX tests FILES ${SOURCE_FILES})

set_target_properties(${TARGET_NAME_TESTS} PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
