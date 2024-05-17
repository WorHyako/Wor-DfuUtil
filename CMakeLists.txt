cmake_minimum_required(VERSION 3.23)

project(Wor-DfuUtil
        VERSION 0.0.1
        LANGUAGES CXX C
        DESCRIPTION "")

add_subdirectory(ThirdParty)

if (NOT ${HaveConfig} OR ${HaveConfig} STREQUAL "")
    return()
endif ()

# --------------- #
#   WorDfuUtils   #
# --------------- #
add_library(WorDfuUtil)

target_compile_features(WorDfuUtil
        PRIVATE cxx_std_17)

file(GLOB Sources src/*.cpp)
target_sources(WorDfuUtil
        PRIVATE ${Sources})

target_include_directories(WorDfuUtil
        PUBLIC include)

target_precompile_headers(WorDfuUtil
        PUBLIC include/pch.hpp)

target_link_libraries(WorDfuUtil
        PUBLIC dfu-util)
