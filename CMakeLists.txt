cmake_minimum_required(VERSION 3.16)

project(Archess)

include_directories(include)

file(GLOB SOURCES
    src/*.cpp
)

add_executable(Archess
    main.cpp
    ${SOURCES}
)

find_package(SDL2 REQUIRED)

target_link_libraries(Archess ncurses SDL2::SDL2)

