add_executable(bootloader_generator
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

set_target_properties(bootloader_generator PROPERTIES
    FOLDER tools
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)
