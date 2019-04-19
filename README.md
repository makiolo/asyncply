# asyncply

gcc 4.9 / clang 3.6: [![Build Status](https://travis-ci.org/makiolo/asyncply.svg?branch=master)](https://travis-ci.org/makiolo/asyncply)

MSVC 2015: [![Build status](https://ci.appveyor.com/api/projects/status/ot5a0ljdt1yqwabp?svg=true)](https://ci.appveyor.com/project/makiolo/asyncply)

Async-Await implementation in C++11/14 for humans

## For Consume this package

### Basic setup

    $ conan install asyncply/1.0.3@npm-mas-mas/testing

### Project setup

If you handle multiple dependencies in your project is better to add a *conanfile.txt*

    [requires]
    asyncply/1.0.3@npm-mas-mas/testing

    [generators]
    cmake

Complete the installation of requirements for your project running:

    $ mkdir build && cd build && conan install ..

Note: It is recommended that you run conan install from a build directory and not the root of the project directory.  This is because conan generates *conanbuildinfo* files specific to a single build configuration which by default comes from an autodetected default profile located in ~/.conan/profiles/default .  If you pass different build configuration options to conan install, it will generate different *conanbuildinfo* files.  Thus, they should not be added to the root of the project, nor committed to git.

## Add Remote

    $ conan remote add npm-mas-mas "https://api.bintray.com/conan/npm-mas-mas/testing"

