# asyncply [![Build Status](https://img.shields.io/shippable/55f433501895ca447414d610/master.svg)](https://app.shippable.com/projects/55f433501895ca447414d610)
Async-Await implementation in C++11/14 for humans

# :construction: I am under construction
Don't use it

# quick-start
```bash
$ git clone --recursive https://github.com/makiolo/asyncply.git asyncply
$ cd asyncply
$ mkdir release
$ cd release
$ cmake .. (default compiler)
$ cmake --build . --config release
$ ctest . -C release
```
# cygwin / mingw
```bash
$ git config --global core.autocrlf false
$ cmake .. -G"MinGW Makefiles" -DCMAKE_C_COMPILER="mingw32-gcc" -DCMAKE_CXX_COMPILER="mingw32-g++" (I use with Mingw)
```
