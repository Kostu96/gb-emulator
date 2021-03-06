### Configure emulator library
set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/base.h
    ${CMAKE_CURRENT_SOURCE_DIR}/cartridge.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cartridge.h
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/input.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/input.h
    ${CMAKE_CURRENT_SOURCE_DIR}/memory_map.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/memory_map.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ppu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ppu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/timer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/timer.h
)

add_library(${TARGET_NAME_LIB} STATIC
	${SOURCE_FILES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${SOURCE_FILES})

target_include_directories(${TARGET_NAME_LIB} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${TARGET_NAME_LIB} PUBLIC
    cpp-common-lib
)

set_target_properties(${TARGET_NAME_LIB} PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

### Configure emulator executable
add_executable(${TARGET_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_include_directories(${TARGET_NAME} PRIVATE
	${SFML_INCLUDE_DIR}
)

target_compile_definitions(${TARGET_NAME} PRIVATE
	SFML_STATIC
)

target_link_libraries(${TARGET_NAME} PRIVATE
    ${TARGET_NAME_LIB}
    ${SFML_LIB_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/sfml-main$<$<CONFIG:Debug>:-d>.lib
    ${SFML_LIB_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/sfml-system-s$<$<CONFIG:Debug>:-d>.lib
    ${SFML_LIB_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/sfml-window-s$<$<CONFIG:Debug>:-d>.lib
    ${SFML_LIB_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/sfml-graphics-s$<$<CONFIG:Debug>:-d>.lib
	${CMAKE_SOURCE_DIR}/third_party/SFML/extlibs/libs-msvc-universal/x64/freetype.lib
	opengl32.lib
	winmm.lib
	gdi32.lib
)

set_target_properties(${TARGET_NAME} PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

if (MSVC)
    set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})
endif()
