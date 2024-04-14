cmake_minimum_required(VERSION 3.23)

add_custom_target(dfu-util_autogen
        COMMAND ./autogen.sh
        COMMENT "Creating dfu-util configuration file..."
        WORKING_DIRECTORY ${DfuUtilRoot})

add_custom_target(dfu-util_configure
        COMMAND ./configure
        DEPENDS dfu-util_autogen
        COMMENT "Configuring dfu-utils for current OS. Generating make files..."
        WORKING_DIRECTORY ${DfuUtilRoot})

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
    set(HaveConfig ON PARENT_SCOPE)
endif ()
