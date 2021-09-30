cmake_minimum_required(VERSION 3.12)

project(pagination)

set(CMAKE_AUTOMOC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS
            Core
            Gui
            Widgets
            REQUIRED)

add_library(${PROJECT_NAME} 
    pagination.h
    pagination.cpp

    pagingutil.h
    pagingutil.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(pagination::lib ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)