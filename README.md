# minesweeper_cpp
## Intro
    minesweeper_cpp is a version of minesweeper that I made in C++ that uses SFML. The goal was to get used to using C++ again as well as the linux environment, cmake, and github. That is it at the moment, but this is going to be my test bench for as many things as possible.

## Requirements
To run this program you will need:
1. CMake
2. SFML

## Build
To build and run:
cmake .
make

## Parameters
This program has a few optional parameters:
1. Board columns, default is 16
2. Board rows, default is 16
3. Bomb count, default is columns * rows * (40 / 256)
4. Draw scale, default is 2

## Issues
    There are no known bugs with the program itself, I'm just not familiar enough with getting SFML to actually use OpenGL. However that is a problem with my environment and once I figure it out I'll write something in here so I remember.
