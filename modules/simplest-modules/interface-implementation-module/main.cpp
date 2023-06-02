import FooMod;

import <iostream>;

int main()
{
    std::cout << "Interface - implementation separation\n";

    FooMod::foo();

    FooMod::Data data{{1, 2, 3, 4, 5}};
    data.print();

    Utils::print(data.items, "data.items");
}