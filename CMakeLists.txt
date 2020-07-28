cmake_minimum_required(VERSION 3.10.2)
project(Minesweeper VERSION 1.0.0)

find_package(SFML 2.4.2 COMPONENTS window graphics system REQUIRED)
add_executable(minesweeper main.cpp Board.cpp)
target_link_libraries(minesweeper sfml-window sfml-graphics sfml-system)
