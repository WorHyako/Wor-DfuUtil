cmake_minimum_required(VERSION 3.23)

set(ConfigComment
        "
/**
 * Wor:
 */")

file(READ dfu-util/config.h Config)
string(FIND "${Config}" "${ConfigComment}" ConfigCommentIndex REVERSE)

if (${ConfigCommentIndex} EQUAL -1)
    file(READ extraConfig.in ExtraConfig)
    string(APPEND Config "${ConfigComment}\n${ExtraConfig}")
    file(WRITE dfu-util/config.h "${Config}")
endif ()
