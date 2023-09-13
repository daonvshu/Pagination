cmake_minimum_required(VERSION 3.12)

project(pagination_pro)

add_subdirectory(src)
add_subdirectory(plugin)

option(PAGINATION_BUILD_TEST "enable pagination test project" OFF)

if (PAGINATION_BUILD_TEST)
    message("config pagination test!")
    add_subdirectory(test)
endif()