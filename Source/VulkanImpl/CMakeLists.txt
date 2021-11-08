cmake_minimum_required(VERSION 3.10)

set(INCLUDE_DIRS Source)
file(GLOB_RECURSE SOURCE_FILES *.cpp *.hpp)
add_library(VulkanImpl SHARED ${SOURCE_FILES})
set_property(TARGET VulkanImpl PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_target_properties(VulkanImpl PROPERTIES FOLDER Implementation)

# Copy the dll to the bin directory
add_custom_command(
    TARGET VulkanImpl
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_BINARY_DIR}/Source/VulkanImpl/$<CONFIGURATION>/VulkanImpl.dll
        ${CMAKE_BINARY_DIR}/Source/Engine/$<CONFIGURATION>
    )