module;

#include <iostream>

export module FooMod;

export void foo();

void foo()
{
    std::cout << "Hello modules\n";
}