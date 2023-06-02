#!/bin/bash

mkdir build
cd build

g++ -std=c++20 -fmodules-ts -xc++-system-header iostream
g++ -std=c++20 -fmodules-ts -xc++-system-header string_view

g++ -std=c++20 -fmodules-ts -xc++ -c ../foo.cxx
g++ -std=c++20 -fmodules-ts -xc++ -c ../foo_impl.cpp
g++ -std=c++20 -fmodules-ts -xc++ -c ../main.cpp
g++ foo.o foo_impl.o main.o -o app

./app