# minesweeper_cpp
## Intro
&nbsp;&nbsp;&nbsp;&nbsp;minesweeper_cpp is a version of minesweeper that I made in C++ that uses SFML. The goal was to get used to using C++ again as well as the Linux environment, CMake, and GitHub. That is it at the moment, but this is going to be my test bench for as many things as possible.

## Requirements
To run this program you will need:
1. CMake
2. SFML

## Build
To build and run:  

```
cmake .
make
```

## Parameters
This program has a few optional parameters:
| Name          | Default                       |
|---------------|-------------------------------|
| Board columns | 16                            |
| Board rows    | 16                            |
| Bomb count    | columns * rows * (40 / 256)   |
| Draw scale    | 2                             |

## Issues
&nbsp;&nbsp;&nbsp;&nbsp;There are no known bugs with the program itself, I'm just not familiar enough with getting SFML to actually use OpenGL. However that is a problem with my environment and once I figure it out I'll write something in here so I remember.
