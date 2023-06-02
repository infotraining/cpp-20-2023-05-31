#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <set>
#include <list>
#include <source_location>
#include <ranges>

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& out, const std::pair<T1, T2>& p)
{
    out << "{" << p.first << "," << p.second << "}";
    return out;
}

#include <catch2/catch_test_macros.hpp>

using namespace std::literals;

std::pair<std::string_view, std::string_view> split(std::string_view line, std::string_view separator = "/")
{
    std::pair<std::string_view, std::string_view> result;

    if (std::string::size_type pos = line.find(separator.data()); pos != std::string::npos)
    {
        result.first = std::string_view{line.begin(), line.begin() + pos};
        result.second = std::string_view{line.begin() + pos + 1, line.end()};
    }

    return result;
}

TEST_CASE("split")
{
    std::string s1 = "324/44";
    CHECK(split(s1) == std::pair{"324"sv, "44"sv});

    std::string s2 = "4343";
    CHECK(split(s2) == std::pair{""sv, ""sv});

    std::string s3 = "345/";
    CHECK(split(s3) == std::pair{"345"sv, ""sv});

    std::string s4 = "/434";
    CHECK(split(s4) == std::pair{""sv, "434"sv});
}

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

TEST_CASE("Exercise - ranges")
{
    const std::vector<std::string_view> lines = {
        "# Comment 1",
        "# Comment 2",
        "# Comment 3",
        "1/one",
        "2/two",
        "\n",
        "3/three",
        "4/four",
        "5/five",
        "\n",
        "\n",
        "6/six"
    };

    print(lines, "lines");

    auto result = lines
        | std::views::drop_while([](const auto& s) { return s.starts_with("#"); })
        | std::views::filter([](const auto& s) { return s != "\n"; })
        | std::views::transform([](const auto& s) { return split(s); })
        | std::views::elements<1>;

    print(result, "result");

    auto expected_result = {"one"s, "two"s, "three"s, "four"s, "five"s, "six"s};

    CHECK(std::ranges::equal(result, expected_result));
}

struct Empty
{
    void foo()
    {
        std::cout << "foo() - " << this << "\n";
    }
};

struct NotEmpty
{
    int x;

    void bar()
    {
        auto src_context = std::source_location::current();
        std::cout << src_context.file_name() << "\n" << src_context.function_name() << "\n" 
            << src_context.line() << "\n";

        std::cout << "bar() - " << this << "\n";
    }
};

struct X
{
    [[no_unique_address]] Empty empty;
    NotEmpty not_empty;
};

TEST_CASE("no_unique_address")
{
    CHECK(sizeof(X) == sizeof(int));
    X x;
    x.empty.foo();
    CHECK(static_cast<void*>(&x.empty) == static_cast<void*>(&x.not_empty));
    x.not_empty.bar();
}