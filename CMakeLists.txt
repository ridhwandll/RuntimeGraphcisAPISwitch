# ------------------------------------
cmake_minimum_required(VERSION 3.10)

project(RuntimeGraphcisAPISwitch LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
# ------------------------------------

#check the compiler
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  # using Clang
  set(CMAKE_CXX_FLAGS "-g3 -O0 -Wall -pedantic -Wextra -m64 -mfma -ffast-math -fno-rtti")
  MESSAGE(STATUS "[Surge] Compiler id ${CMAKE_CXX_COMPILER_ID}")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using GCC
  set(CMAKE_CXX_FLAGS "-Wall -pedantic -Wextra -m64 -mfma -ffast-math -fno-rtti")
  MESSAGE(STATUS "[Surge] Compiler id ${CMAKE_CXX_COMPILER_ID}")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # using Visual Studio C++
  set(CMAKE_CXX_FLAGS "/W3 /fp:fast /GR- /MP /EHsc")
  MESSAGE(STATUS "[Surge] Compiler id ${CMAKE_CXX_COMPILER_ID}(The best compiler)")


endif()
add_subdirectory(Source)
add_subdirectory(Vendor/benchmark)
set_target_properties(benchmark PROPERTIES FOLDER Dependencies)
set_target_properties(benchmark_main PROPERTIES FOLDER Dependencies)
target_include_directories(Engine PRIVATE Vendor)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Engine)
