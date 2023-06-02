#include <algorithm>
#include <array>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

using namespace std::literals;

template <std::ranges::range TRng>
void print(TRng&& rng, std::string_view prefix = "")
    // requires requires{ std::cout << std::declval<std::ranges::range_value_t<TRng>>(); }
    requires requires(std::ranges::range_value_t<TRng>& item) { std::cout << item; }
{
    std::cout << prefix << " [";
    for (const auto& item : rng)
    {
        std::cout << item << " ";
    }
    std::cout << "]\n";
}

template <typename T = void>
struct Greater
{
    bool operator()(const T& a, const T& b) const
    {
        return a > b;
    }
};

template <>
struct Greater<void>
{
    template <typename T1, typename T2>
    bool operator()(const T1& a, const T2& b) const
    {
        return a > b;
    }
};

TEST_CASE("ranges")
{
    std::vector data = {5, 423, 665, 42, 1, 235, 6, 345, 33};
    print(data, "data");

    std::ranges::sort(data);
    print(data, "data after sort");

    // std::ranges::sort(data, [](const auto& a, const auto& b) { return a > b; });
    std::ranges::sort(data, std::greater{});
    print(data, "data sorted desc");

    std::vector words = {"twenty-two"s, "a"s, "abc"s, "b"s, "one"s, "aa"s};
    std::ranges::sort(words, std::less{}, /*projection*/ [](const auto& w) { return w.size(); });
    print(words, "words");
}

TEST_CASE("begin & end")
{
    std::vector data = {5, 423, 665, 42, 1, 235, 6, 345, 33};
    print(data, "data");

    auto result = std::accumulate(std::ranges::begin(data), std::ranges::end(data), 0);
}

/////////////////////
// Sentinels

template <auto Value>
struct EndValue
{
    bool operator==(auto it) const
    {
        return *it == Value;
    }
};

void print_with_sentinel(auto first, auto last)
{
    std::cout << "[ ";
    for (auto it = first; it != last; ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << "]\n";
}

TEST_CASE("sentinels")
{
    EndValue<0> ev;           // EndValue<int Value = 0>
    EndValue<'\0'> null_term; // EndValue<char = '\0'>

    std::array<int, 1024> data = {1, 2, 3, 4};

    print_with_sentinel(data.begin(), ev);

    auto& txt = "acbgdef\0ajdhfgajsdhfgkasdjhfg";
    auto txt_array = std::to_array("abcgdef\0ajdhfgajsdhfgkasdjhfg");
    std::ranges::sort(std::ranges::begin(txt_array), null_term);
    print_with_sentinel(std::ranges::begin(txt_array), null_term);

    std::vector vec = {1, 2, 3, 4, 5, 42, 6, 7, 8, 9, 10};
    std::ranges::sort(vec.begin(), EndValue<42>{}, std::greater{});
    print(vec, "vec");

    auto head_data = std::ranges::subrange(vec.begin(), EndValue<42>{});
    print(head_data, "head-data");
}

TEST_CASE("views")
{
    std::vector vec = {1, 2, 3, 4, 5, 42, 6, 7, 8, 9, 10};

    SECTION("all")
    {
        auto all_items = std::views::all(vec);
        print(all_items, "all-items");
    }

    SECTION("counted")
    {
        auto first_half = std::views::counted(vec.begin(), vec.size() / 2);
        print(first_half, "first-half");

        for (auto& item : first_half)
            item *= 2;

        print(vec, "vec");
    }

    SECTION("iota")
    {
        print(std::views::iota(1, 20), "iota");

        auto numbers = std::views::iota(1) | std::views::take(15);

        print(numbers, "numbers");

        auto data = std::views::iota(1)
            | std::views::take(100)
            | std::views::filter([](int x) { return x % 2 == 0; })
            | std::views::transform([](int x) { return x * x; });

        print(data, "data");

        print(data | std::views::take(3), "first-3");
    }

    SECTION("views - reference semantics")
    {
        std::vector vec = {1, 2, 3, 4, 5, 42, 6, 7, 8, 9, 10};

        auto pos1 = std::ranges::find(vec, 42);
        auto pos2 = std::ranges::find(vec.begin(), std::unreachable_sentinel, 42);
        CHECK(pos1 == pos2);

        for (auto& item : vec | std::views::drop(5))
            item = 0;

        print(vec, "vec");
    }
}

std::vector<int> get_data()
{
    // std::vector vec{1, 2, 3, 42, 22};
    // return vec; // lvale - optional NRVO

    return {1, 2, 3, 42, 22}; // prvalue - must be RVO
}

TEST_CASE("borrowed iterators")
{
    SECTION("dangling iterator")
    {
        auto pos = std::ranges::find(get_data(), 42);

        // REQUIRE(*pos == 42); // Compilation ERROR - dangling iterator
    }

    SECTION("valid iterator")
    {
        auto data = get_data();
        auto pos = std::ranges::find(data, 42);

        data.clear

        REQUIRE(*pos == 42);
    }
}