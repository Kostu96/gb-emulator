### Configure emulator library
# Generate bootloader
set(BOOTLOADER_FILE ${CMAKE_CURRENT_BINARY_DIR}/generated_sources/bootloader.inl)

add_custom_command(
	OUTPUT ${BOOTLOADER_FILE}
	COMMAND bootloader_generator ${CMAKE_SOURCE_DIR}/assets/DMG_ROM.bin ${BOOTLOADER_FILE} ${CMAKE_SOURCE_DIR}/assets/bootloader_patches
	DEPENDS bootloader_generator
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/cartridge.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cartridge.h
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/memory_map.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/memory_map.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ppu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ppu.h
)

add_library(${TARGET_NAME_LIB} STATIC
	${BOOTLOADER_FILE}
	${SOURCE_FILES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${SOURCE_FILES})

target_include_directories(${TARGET_NAME_LIB} PRIVATE
	${CMAKE_CURRENT_BINARY_DIR}/generated_sources
)

target_link_libraries(${TARGET_NAME_LIB} PRIVATE
    cpp-common-lib
)

set_target_properties(${TARGET_NAME_LIB} PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)



### Configure emulator executable
add_executable(${TARGET_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(${TARGET_NAME}
    ${TARGET_NAME_LIB}
    sfml-graphics
)

set_target_properties(${TARGET_NAME} PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

if (MSVC)
    set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})
endif()
