# asyncply [![Build Status](https://img.shields.io/shippable/55f433501895ca447414d610/master.svg)](https://app.shippable.com/projects/55f433501895ca447414d610)
Async-Await implementation in C++11/14 for humans

# :construction: I am under construction
Don't use it

# checkout and prepare
```bash
$ git clone --recursive https://github.com/makiolo/asyncply.git asyncply
$ cd asyncply
$ mkdir release
$ cd release
```
# Option 1: use default compiler
```bash
$ cmake ..
```
# Option 2: use specific compiler
```bash
$ cmake .. -G"MinGW Makefiles" -DCMAKE_C_COMPILER="mingw32-gcc" -DCMAKE_CXX_COMPILER="mingw32-g++"
$ cmake .. -G"MinGW Makefiles" -DCMAKE_C_COMPILER="x86_64-w64-mingw32-gcc" -DCMAKE_CXX_COMPILER="x86_64-w64-mingw32-g++"
$ 
```
# compilation and execute unittests
```bash
$ cmake --build . --config release
$ ctest . -C release
```
