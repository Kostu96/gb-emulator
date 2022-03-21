set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/cartridge.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cartridge.h
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu_instructions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/helper_functions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/helper_functions.h
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/memory_map.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/memory_map.h
)

#add_library(${TARGET_NAME_LIB} STATIC
#    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
#	${SOURCE_FILES}
#)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${SOURCE_FILES})

#target_link_libraries(${TARGET_NAME_LIB}
#	glad
#	glfw
#)

#set_target_properties(${TARGET_NAME_LIB} PROPERTIES
#    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
#)

add_executable(${TARGET_NAME}
	${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${SOURCE_FILES}
)

#target_link_libraries(${TARGET_NAME}
#    ${TARGET_NAME_LIB}
#)

set_target_properties(${TARGET_NAME} PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

if (MSVC)
    set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})
endif()
