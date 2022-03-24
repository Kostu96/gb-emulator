add_executable(bootloader_generator
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(bootloader_generator PRIVATE
    cpp-common-lib
)

set_target_properties(bootloader_generator PROPERTIES
    FOLDER tools
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)
