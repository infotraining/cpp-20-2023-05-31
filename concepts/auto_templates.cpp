#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std::literals;

void print(auto&& rng, std::string_view prefix = "items")
{
    std::cout << prefix << ": [ ";
    for (const auto& item : rng)
        std::cout << item << " ";
    std::cout << "]\n";
}

auto cmp_by_size(const auto& l, const auto& r)
{
    return l.size() < r.size();
}

bool less(const auto& a, const auto& b)
{
    return a < b;
}

namespace HowItWorks
{
    template <typename T1, typename T2>
    bool less(const T1& a, const T2& b)
    {
        return a < b;
    }

    template <typename T1, typename T2>
    bool cmp_by_size(T1 l, T2 r)
    {
        return l.size() < r.size();
    }
} // namespace HowItWorks

struct Gadget
{
    std::string name;

    void print(const auto& prefix)
    {
        std::cout << prefix << " - " << name << "\n";
    }
};

TEST_CASE("abbreviated template functions")
{
    auto cmp_by_size_lambda = [](const auto& l, const auto& r) {
        return l.size() < r.size();
    };

    std::string str = "abc";
    std::vector vec{1, 2, 3, 4};

    CHECK(cmp_by_size(str, vec) == true);

    std::vector<std::string> words = {"twenty-two", "zero", "one", "two", "three", "four"};

    //std::sort(words.begin(), words.end(), cmp_by_size_lambda);                    // OK

    std::sort(words.begin(), words.end(), cmp_by_size<std::string, std::string>); // function template requires args

    print(words, "words");

    Gadget g{"ipad"};
    g.print("Gadget");
}