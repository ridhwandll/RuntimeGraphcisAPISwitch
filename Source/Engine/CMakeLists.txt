cmake_minimum_required(VERSION 3.10)

set(INCLUDE_DIRS Source)
file(GLOB_RECURSE SOURCE_FILES *.cpp *.hpp *.h)
add_executable(Engine ${SOURCE_FILES})
set_property(TARGET Engine PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_target_properties(Engine PROPERTIES FOLDER Engine)
target_compile_definitions(Engine PRIVATE EXE)
target_link_libraries(Engine benchmark)

if (UNIX)
    target_link_libraries(Engine dl)
endif (UNIX)

# Copy the dlls to the bin directory
if (WIN32)
    add_custom_command(
        TARGET Engine
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_BINARY_DIR}/Source/DirectX12Impl/$<CONFIGURATION>/DirectX12Impl.dll
            ${CMAKE_BINARY_DIR}/Source/Engine/$<CONFIGURATION>
        )
    add_custom_command(
        TARGET Engine
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_BINARY_DIR}/Source/VulkanImpl/$<CONFIGURATION>/VulkanImpl.dll
            ${CMAKE_BINARY_DIR}/Source/Engine/$<CONFIGURATION>
    )
endif (WIN32)

if (UNIX)
    add_custom_command(
        TARGET Engine
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_BINARY_DIR}/Source/DirectX12Impl/$<CONFIGURATION>/libDirectX12Impl.so
            ${CMAKE_BINARY_DIR}/Source/Engine/$<CONFIGURATION>
        )
    add_custom_command(
        TARGET Engine
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_BINARY_DIR}/Source/VulkanImpl/$<CONFIGURATION>/libVulkanImpl.so
            ${CMAKE_BINARY_DIR}/Source/Engine/$<CONFIGURATION>
    )
endif(UNIX)
