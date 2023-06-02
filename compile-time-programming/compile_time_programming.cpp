#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

using namespace std::literals;

int runtime_func(int x)
{
    return x * x;
}

constexpr int constexpr_func(int x)
{
    return x * x;
}

consteval int consteval_func(int x)
{
    return x * x;
}

TEST_CASE("run and compile time")
{
    int x = 42;
    int rx = runtime_func(x);

    // constexpr int cx = runtime_func(x);
    constexpr auto cvar = 42;
    constexpr int cx = constexpr_func(cvar); // compile time
    std::cout << constexpr_func(x) << "\n";  // runtime

    std::cout << consteval_func(42) << "\n";
    std::cout << consteval_func(cvar) << "\n";
}

//template <size_t N>
constexpr auto create_powers(auto size)
{
    std::array<uint32_t, size()> powers{};

    std::iota(powers.begin(), powers.end(), 1);                                  // std algorithms are constexpr

    std::ranges::transform(powers, powers.begin(), [](int x) { return x * x; }); // ranges algorithms are constexpr

    return powers;
}

TEST_CASE("constexpr extensions")
{
    int tab[consteval_func(8)];

    constexpr auto lookup_squares = create_powers([]{ return 12;});

    static_assert(lookup_squares[3] == 16);
}

template <std::ranges::input_range... TRng_>
constexpr auto avg_for_unique(const TRng_&... rng)
{
    using TElement = std::common_type_t<std::ranges::range_value_t<TRng_>...>;

    std::vector<TElement> vec;                            // empty vector
    vec.reserve((... + rng.size()));                      // reserve a buffer
    (vec.insert(vec.end(), rng.begin(), rng.end()), ...); // fold expression C++17

    // sort items
    std::ranges::sort(vec); // std::sort(vec.begin(), vec.end());

    // create span of unique_items
    auto new_end = std::unique(vec.begin(), vec.end());
    std::span unique_items{vec.begin(), new_end};

    // calculate sum of unique items
    auto sum = std::accumulate(unique_items.begin(), unique_items.end(), TElement{});

    return sum / static_cast<double>(unique_items.size());
}

TEST_CASE("avg for unique")
{
    constexpr std::array lst1 = {1, 2, 3, 4, 5};
    constexpr std::array lst2 = {5, 6, 7, 8, 9};

    constexpr auto avg = avg_for_unique(lst1, lst2);

    std::cout << "AVG: " << avg << "\n";
}

template <typename... TArgs>
constexpr auto sum(TArgs... args) // 1, 2, 3
{
    return (... + args); // (1 + (2 + (3))))
}

TEST_CASE("folds")
{
    static_assert(sum(1, 2, 3, 4) == 10);
}

const constinit int VALUE = sum(1, 5, 6, 7, 8);

TEST_CASE("constinit")
{
    //++VALUE;
    std::cout << "VALUE: " << VALUE << "\n";

    //++VALUE;
    std::cout << "VALUE: " << VALUE << "\n";
}