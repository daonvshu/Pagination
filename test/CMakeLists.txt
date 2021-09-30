cmake_minimum_required(VERSION 3.12)

project(pagination_test)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS
            Core
            Gui
            Widgets
            REQUIRED)

add_executable(${PROJECT_NAME} WIN32
    main.cpp

    paginationtest.h
    paginationtest.cpp
    paginationtest.ui
)

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    pagination::lib
)