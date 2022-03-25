set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(${TARGET_NAME_TESTS}
	${SOURCE_FILES}
)

target_include_directories(${TARGET_NAME_TESTS} PRIVATE
	${GTEST_INCLUDE_DIR}
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
