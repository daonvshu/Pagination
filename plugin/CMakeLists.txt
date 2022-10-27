cmake_minimum_required(VERSION 3.12)

project(pagination_plugin)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)

add_library(${PROJECT_NAME} SHARED
    paginationplugin.h
    paginationplugin.cpp
    paginationplugin.qrc
)

target_link_libraries(${PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    pagination::lib
)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(TARGET_OUTPUT_NAME "pagination_plugin_d")
else()
    set(TARGET_OUTPUT_NAME "pagination_plugin")
endif()
message("config output name: ${TARGET_OUTPUT_NAME}")

message("qt5 designer dir: ${Qt${QT_VERSION_MAJOR}_DIR}/../../../plugins/designer")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${Qt${QT_VERSION_MAJOR}_DIR}/../../../plugins/designer/${TARGET_OUTPUT_NAME}.dll
)