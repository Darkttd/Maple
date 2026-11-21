# CMake build for Linux and MinGW-w64; the Visual Studio projects in Source/Game are separate.
cmake_minimum_required(VERSION 3.16)
project(Maple LANGUAGES C CXX)

set(BIBIM_BUILD_TESTS OFF CACHE BOOL "Build the Bibim engine unit tests")
add_subdirectory(Source/bibim)

file(GLOB_RECURSE MAPLE_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/Maple/*.cpp)
add_executable(Maple WIN32 ${MAPLE_SOURCES})
target_include_directories(Maple PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Source/Game)
target_link_libraries(Maple PRIVATE Bibim)

if(MINGW)
    # Carry the MinGW runtime so Maple.exe runs without its DLLs.
    target_link_options(Maple PRIVATE -static)
endif()
