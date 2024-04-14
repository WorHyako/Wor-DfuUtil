cmake_minimum_required(VERSION 3.23)

# ---------- #
#   libusb   #
# ---------- #
find_path(LibusbIncludeDir
        NAMES libusb.h
        PATH_SUFFIXES "include" "libusb" "libusb-1.0")
find_library(LibusbLib
        NAMES libusb-1.0.dylib
        PATH_SUFFIXES "lib" "lib32" "lib64")

add_library(libusb INTERFACE)
target_link_libraries(libusb
        INTERFACE ${LibusbLib})
target_include_directories(libusb
        INTERFACE ${LibusbIncludeDir})

# ------------ #
#   dfu-util   #
# ------------ #
project(dfu-util
        LANGUAGES C)

set(DfuUtilRoot ${CMAKE_CURRENT_SOURCE_DIR}/dfu-util)
set(DfuUtilSourceDir ${DfuUtilRoot}/src)

include(DfuUtilConfiguring.cmake)

add_library(dfu-util)

target_compile_definitions(dfu-util
        PUBLIC -DHAVE_CONFIG_H)

set(Sources
        ${DfuUtilSourceDir}/dfu_load.c
        ${DfuUtilSourceDir}/dfu_util.c
        ${DfuUtilSourceDir}/dfuse.c
        ${DfuUtilSourceDir}/dfuse_mem.c
        ${DfuUtilSourceDir}/dfu.c
        ${DfuUtilSourceDir}/dfu_file.c
        ${DfuUtilSourceDir}/quirks.c)

source_group("dfu-util_sources"
        FILES ${Sources})
target_sources(dfu-util
        PRIVATE
        ${Sources}
        DfuUtilVariables.cpp)

target_include_directories(dfu-util
        PUBLIC
        ${DfuUtilRoot}/src
        ${DfuUtilRoot})

target_link_libraries(dfu-util
        PUBLIC libusb)

# ---------------- #
#   dfu-util_exe   #
# ---------------- #
add_executable(dfu-util_exe)
target_compile_definitions(dfu-util_exe
        PUBLIC -DHAVE_CONFIG_H)

target_sources(dfu-util_exe
        PRIVATE
        ${Sources}
        ${DfuUtilSourceDir}/main.c)

target_include_directories(dfu-util_exe
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${DfuUtilRoot}/src
        ${DfuUtilRoot})

target_link_libraries(dfu-util_exe
        PUBLIC libusb)
