#!/bin/bash

mkdir build
cd build

g++ -std=c++20 -fmodules-ts -xc++-system-header iostream
g++ -std=c++20 -fmodules-ts -xc++-system-header string
g++ -std=c++20 -fmodules-ts -xc++-system-header vector

g++ --std=c++20 -fmodules-ts -xc++ -c ../shapes_point.cxx
g++ --std=c++20 -fmodules-ts -xc++ -c ../shapes_base.cxx
g++ --std=c++20 -fmodules-ts -xc++ -c ../shapes.cxx
g++ --std=c++20 -fmodules-ts -xc++ -c ../main.cpp
g++ shapes.o shapes_point.o shapes_base.o main.o -o shapes_app

./shapes_app