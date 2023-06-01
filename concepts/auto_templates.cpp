#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <ranges>

using namespace std::literals;

template <typename T>
concept PrintableRange = std::ranges::range<T> && requires {
    std::cout << std::declval<std::ranges::range_value_t<T>>();
};

void print(PrintableRange auto&& rng, std::string_view prefix = "items")
{
    std::cout << prefix << ": [ ";
    for (const auto& item : rng)
        std::cout << item << " ";
    std::cout << "]\n";
}

TEST_CASE("printing with concepts")
{
    std::vector<int> vec{1, 2, 3};
    print(vec, "vec");

    // std::map<int, int> mp {{1, 1}, {2, 2}};
    // print(mp, "mp");
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

    auto cmp_by_size_lambda_20 = []<typename T>(const std::vector<T>& l, const std::vector<T>& r) {
        return l.size() < r.size();
    };

    std::string str = "abc";
    std::vector vec{1, 2, 3, 4};

    CHECK(cmp_by_size(str, vec) == true);

    std::vector<std::string> words = {"twenty-two", "zero", "one", "two", "three", "four"};

    // std::sort(words.begin(), words.end(), cmp_by_size_lambda);                    // OK

    std::sort(words.begin(), words.end(), cmp_by_size<std::string, std::string>); // function template requires args

    print(words, "words");

    Gadget g{"ipad"};
    g.print("Gadget");
}

template <typename F, typename... TArgs>
decltype(auto) call_wrapper(F f, TArgs&&... args)
{
    std::cout << "calling a function!!!" << std::endl;

    return f(std::forward<TArgs>(args)...); // perfect forwarding
}

namespace Cpp20
{
    decltype(auto) call_wrapper(auto f, auto&&... args)
    {
        std::cout << "calling a function!!!" << std::endl;

        return f(std::forward<decltype(args)>(args)...); // perfect forwarding
    }

    const static inline auto caller = []<typename... TArgs>(auto f, TArgs&&... args) {
        std::cout << "calling a function!!!" << std::endl;
        return f(std::forward<TArgs>(args)...);
    };
} // namespace Cpp20

inline int foo(int x)
{
    return x * 42;
}

TEST_CASE("call_wrapper")
{
    CHECK(Cpp20::call_wrapper(foo, 2) == 84);
    CHECK(Cpp20::caller(foo, 2) == 76);
}
