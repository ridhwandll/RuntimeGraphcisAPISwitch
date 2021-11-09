cmake_minimum_required(VERSION 3.10)

set(INCLUDE_DIRS Source)
file(GLOB_RECURSE SOURCE_FILES *.cpp *.hpp)
add_library(DirectX12Impl SHARED ${SOURCE_FILES})
set_property(TARGET DirectX12Impl PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_target_properties(DirectX12Impl PROPERTIES FOLDER Implementation)

# Copy the dll to the bin directory
if(WIN32)
add_custom_command(
    TARGET DirectX12Impl
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_BINARY_DIR}/Source/DirectX12Impl/$<CONFIGURATION>/DirectX12Impl.dll
        ${CMAKE_BINARY_DIR}/Source/Engine/$<CONFIGURATION>
    )
endif(WIN32)

if(UNIX)
add_custom_command(
    TARGET DirectX12Impl
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_BINARY_DIR}/Source/DirectX12Impl/$<CONFIGURATION>/libDirectX12Impl.so
        ${CMAKE_BINARY_DIR}/Source/Engine/$<CONFIGURATION>
    )
endif(UNIX)
