# asyncply

gcc 4.9 / clang 3.6: [![Build Status](https://travis-ci.org/makiolo/asyncply.svg?branch=master)](https://travis-ci.org/makiolo/asyncply)

MSVC 2015: [![Build status](https://ci.appveyor.com/api/projects/status/ot5a0ljdt1yqwabp?svg=true)](https://ci.appveyor.com/project/makiolo/asyncply)

Async-Await implementation in C++11/14 for humans

# :construction: I am under construction
Don't use it

# checkout and prepare
```bash
$ git clone --recursive https://github.com/makiolo/asyncply.git
$ ./cmaki/run.sh
```
# Option 1: use default compiler
```bash
$ cmake ..
```
# Option 2: use specific compiler
```bash
$ cmake .. -G"MinGW Makefiles" -DCMAKE_C_COMPILER="mingw32-gcc" -DCMAKE_CXX_COMPILER="mingw32-g++"
$ cmake .. -G"MinGW Makefiles" -DCMAKE_C_COMPILER="x86_64-w64-mingw32-gcc" -DCMAKE_CXX_COMPILER="x86_64-w64-mingw32-g++"
```
# compilation and execute unittests
```bash
$ cmake --build . --config release
$ ctest . -C release
```
