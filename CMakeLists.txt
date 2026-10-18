cmake_minimum_required(VERSION 3.10)
project(1CSelection) # TODO

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src/dungeon)
add_subdirectory(src/interaction)

add_executable(main ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(main PRIVATE dungeon)
target_link_libraries(main PRIVATE interaction)
