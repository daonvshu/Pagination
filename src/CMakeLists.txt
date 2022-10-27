cmake_minimum_required(VERSION 3.12)

project(pagination)

set(CMAKE_AUTOMOC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
            Core
            Gui
            Widgets)

add_library(${PROJECT_NAME} 
    pagination.h
    pagination.cpp

    pagingutil.h
    pagingutil.cpp

    pagingstyle1.h
    pagingstyle1.cpp
    pagingstyle2.h
    pagingstyle2.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(pagination::lib ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
)