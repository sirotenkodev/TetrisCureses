cmake_minimum_required(VERSION 3.14)

project(TetrisCureses LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-lncurses")

find_package(Curses QUIET)

if(NOT Curses_FOUND)
    message(STATUS "Curses not found, downloading with FetchContent...")

    include(FetchContent)

    FetchContent_Declare(
        ncurses
        GIT_REPOSITORY https://github.com/Kitware/Curses.git
        GIT_TAG master
    )

    FetchContent_GetProperties(ncurses)
    if(NOT ncurses_POPULATED)
        FetchContent_Populate(ncurses)
        add_subdirectory(${ncurses_SOURCE_DIR} ${ncurses_BINARY_DIR})
    endif()

    find_package(Curses REQUIRED)
endif()

if(WIN32)
    message(STATUS "Building on Windows, downloading PDCurses...")

    FetchContent_Declare(
        pdcurses
        GIT_REPOSITORY https://github.com/essentialen/pdcurses.git
        GIT_TAG master
    )

    FetchContent_GetProperties(pdcurses)
    if(NOT pdcurses_POPULATED)
        FetchContent_Populate(pdcurses)
        add_subdirectory(${pdcurses_SOURCE_DIR} ${pdcurses_BINARY_DIR})
    endif()

    add_definitions(-DNCURSES_WIDECHAR)
    include_directories(${pdcurses_SOURCE_DIR})
    target_link_libraries(${PROJECT_NAME} PRIVATE PDCurses)
endif()

set(CMAKE_THREAD_LIBS_INIT)

include_directories(${CURSES_INCLUDE_DIR})

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE ${CURSES_LIBRARIES})
