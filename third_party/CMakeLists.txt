add_subdirectory(cpp-common)
set_target_properties(cpp-common-lib PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)
