module;

export module FooMod;

import <iostream>;
import <string_view>;

export namespace FooMod
{
    struct Data
    {
        int items[10];

        void print() const;
    };

    void foo();
}

namespace Utils
{
    export
    template <typename TRange>
    void print(TRange&& rng, std::string_view prefix = "items")
    {
        std::cout << prefix << ": [ ";
        for(const auto& item : rng)
            std::cout << item << " ";
        std::cout << "]\n";
    }
}