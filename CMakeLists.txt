cmake_minimum_required(VERSION 3.10)
project(1CSelection) # TODO

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src/dungeon)

add_executable(main ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/dungeon)
target_link_libraries(main PRIVATE dungeon)

# add_library(
#   transformer
#   ${CMAKE_CURRENT_SOURCE_DIR}/src/transformer.hpp
#   ${CMAKE_CURRENT_SOURCE_DIR}/src/transformer.cpp
# )

# target_compile_options(transformer PUBLIC 
#     -g
#     -Wall
#     -Wextra
#     -march=native
# )

# target_link_libraries(transformer PUBLIC vertex)

# target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/)
# target_link_libraries(main vertex transformer scheme)

# add_executable(unit ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit.cpp)
# target_include_directories(unit PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/)
# target_link_libraries(unit vertex transformer scheme gtest_main gmock)
# add_test(NAME SchemeUnitTest COMMAND unit)

# add_executable(stress ${CMAKE_CURRENT_SOURCE_DIR}/tests/stress.cpp)
# target_include_directories(stress PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/)
# target_link_libraries(stress vertex transformer scheme gtest_main gmock)
# add_test(NAME SchemeStressTest COMMAND stress)