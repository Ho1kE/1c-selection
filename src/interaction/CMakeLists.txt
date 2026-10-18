add_library(interaction
interaction.cpp
interaction.hpp)

target_include_directories(interaction PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(interaction PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(interaction PRIVATE dungeon)