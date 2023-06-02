#!/bin/bash

mkdir build
cd build

g++ --std=c++20 -fmodules-ts -c ../primes.cpp
g++ --std=c++20 -fmodules-ts -c ../client_primes.cpp
g++ -std=c++20 client_primes.o primes.o -o primes_main

./primes_main