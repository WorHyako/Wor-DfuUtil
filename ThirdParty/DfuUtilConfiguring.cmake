cmake_minimum_required(VERSION 3.23)

find_path(LibusbIncludeDir REQUIRED
        NAMES libusb.h
        PATH_SUFFIXES "include" "libusb" "libusb-1.0")
find_path(LibusbLibDir REQUIRED
        NAMES libusb-1.0.dylib libusb-1.0.lib
        PATH_SUFFIXES "lib" "lib32" "lib64")

if (APPLE)
    set(AutogenCommand ./autogen.sh)
    set(ConfigureCommand ./configure --libdir=${DfuRoot}/lib --includedir=${DfuRoot}/include)
elseif (WIN32)
    set(AutogenCommand bash autogen.sh)
    set(ConfigureCommand bash configure USB_CFLAGS="-I${LibusbIncludeDir}" USB_LIBS="-L ${LibusbLibDir} -lusb-1.0")
endif ()

add_custom_target(dfu-util_autogen
        COMMAND ${AutogenCommand}
        COMMENT "Creating dfu-util configuration file..."
        WORKING_DIRECTORY ${DfuUtilRoot})

add_custom_target(dfu-util_configure
        COMMAND ${ConfigureCommand}
        DEPENDS dfu-util_autogen
        COMMENT "Configuring dfu-utils for current OS. Generating make files..."
        WORKING_DIRECTORY ${DfuUtilRoot})

include(WriteExtraConfig.cmake)

add_custom_command(TARGET dfu-util_configure
        POST_BUILD
        COMMENT "Adding extra config to config.h file..."
        COMMAND ${CMAKE_COMMAND}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/WriteExtraConfig.cmake
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_target(dfu-util_build
        COMMAND make
        DEPENDS dfu-util_configure
        COMMENT "Running make for dfu-utils project..."
        WORKING_DIRECTORY ${DfuUtilRoot})

if (NOT EXISTS ${DfuUtilRoot}/config.h)
    message(WARNING "Cannot find dfu-util's config file. Configure dfu-util first.
    Run build dfu-util_configure target.")
    return()
else ()
    set(HaveConfig ON)
endif ()
