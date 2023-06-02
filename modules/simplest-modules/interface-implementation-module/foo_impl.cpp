module;

import <iostream>;

module FooMod;

const char* get_text()
{
    return "Hello modules";
}

void FooMod::foo()
{
    std::cout << get_text() << "\n";
}

void FooMod::Data::print() const
{
    Utils::print(items);
}