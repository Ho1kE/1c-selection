add_library(dungeon
dungeon.cpp
dungeon.hpp)

target_include_directories(dungeon PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(dungeon PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})