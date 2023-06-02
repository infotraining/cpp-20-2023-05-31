#!/bin/bash

mkdir build
cd build

g++ --std=c++20 -fmodules-ts -xc++ -c ../foo.cxx
g++ --std=c++20 -fmodules-ts -xc++ -c ../main.cpp
g++ foo.o main.o -o app

./app