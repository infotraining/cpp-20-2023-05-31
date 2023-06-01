#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <set>
#include <list>
#include <source_location>

using namespace std::literals;

std::pair<std::string_view, std::string_view> split(const std::string& line, std::string_view separator = "/")
{
    std::pair<std::string_view, std::string_view> result;

    if (std::string::size_type pos = line.find(separator.data()); pos != std::string::npos)
    {
        result.first = std::string_view{line.begin(), line.begin() + pos};
        result.second = std::string_view{line.begin() + pos + 1, line.end()};
    }

    return result;
}

TEST_CASE("Exercise - ranges")
{
    const std::vector<std::string> lines = {
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

    auto result = lines; // | TODO

    auto expected_result = {"one"s, "two"s, "three"s, "four"s, "five"s, "six"s};

    CHECK(std::ranges::equal(result, expected_result));
}