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

## Note to Self
Windows Subsystem for Linux does not like the -wgl flag for VcXsrv which is the check box for using native OpenGL in XLaunch. Using that makes WSL fail to us OpenGL and the program runs very poorly. Don't forget about this.
