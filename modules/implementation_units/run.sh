#!/bin/bash

mkdir build
cd build

g++ -std=c++20 -fmodules-ts -xc++-system-header iostream
g++ -std=c++20 -fmodules-ts -xc++-system-header string
g++ -std=c++20 -fmodules-ts -xc++-system-header vector
g++ -std=c++20 -fmodules-ts -xc++-system-header list

g++ -std=c++20 -fmodules-ts -c ../eshop.cpp
g++ -std=c++20 -fmodules-ts -c ../eshop_price_impl.cpp
g++ -std=c++20 -fmodules-ts -c ../eshop_io_impl.cpp
g++ -std=c++20 -fmodules-ts -c ../main.cpp
g++ eshop.o eshop_price_impl.o eshop_io_impl.o main.o -o eshopper

./eshopper