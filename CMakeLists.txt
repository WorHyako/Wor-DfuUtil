cmake_minimum_required(VERSION 3.23)

project(Wor-DfuUtils
        LANGUAGES CXX)

add_subdirectory(ThirdParty)

if (NOT ${HaveConfig})
    return()
endif ()

# --------------- #
#   WorDfuUtils   #
# --------------- #
add_executable(WorDfuUtils)

file(GLOB Sources src/*.cpp)
target_sources(WorDfuUtils
        PRIVATE ${Sources})

target_compile_options(WorDfuUtils
        PRIVATE -std=c++17)

target_include_directories(WorDfuUtils
        PRIVATE include
        ThirdParty/WorLibrary/Include)

target_precompile_headers(WorDfuUtils
        PRIVATE include/pch.hpp)

target_link_libraries(WorDfuUtils
        PRIVATE dfu-util)
