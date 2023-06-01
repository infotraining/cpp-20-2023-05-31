#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <span>
#include <string>
#include <vector>
#include <format>

using namespace std::literals;

void print_items(std::span<int> data)
{
    for (const auto& item : data)
        std::cout << item << " ";
    std::cout << "\n";
}

TEST_CASE("std::span")
{
    SECTION("fixed extent")
    {
        int raw_data[32] = {1, 2, 3, 4, 5};

        std::span<int, 32> all_data(raw_data, 32);

        int i = 0;
        for (auto& item : all_data)
        {
            item = ++i;
        }

        print_items(all_data);
    }

    SECTION("dynamic extent")
    {
        std::vector vec = {1, 2, 3, 4, 5, 6, 7, 8};

        std::span<int> all_data(vec);

        //vec.erase(vec.begin()); // UB
        //CHECK(all_data.size() == 7); // ERROR        

        print_items(all_data);

        print_items(std::span{vec.begin() + 2, 4});

        print_items(vec);

        int raw_data[10] = {42, 665, 13};

        print_items(std::span{raw_data});

        int* dynamic_data = new int[32];

        print_items(std::span{dynamic_data, 32});        

        all_data = std::span{dynamic_data, 6};

        for(auto& item : all_data)
            item = -1;

        print_items(std::span{dynamic_data, 32});

        delete[] dynamic_data;
    }
}

void print_as_bytes(const float f, const std::span<const std::byte> bytes)
{
	std::cout << std::format("{:+6}", f) << " - { ";

	for(const std::byte b : bytes)
	{
		std::cout << std::format("{:02X} ", std::to_integer<int>(b));
	}

	std::cout << "}\n";
}

TEST_CASE("span of bytes")
{
	float data[] = { 3.141592f };

	auto const const_bytes = std::as_bytes(std::span{data});
	print_as_bytes(data[0], const_bytes);

	auto const writable_bytes = std::as_writable_bytes(std::span{data});
	writable_bytes[3] |= std::byte{ 0b1000'0000 };
	print_as_bytes(data[0], writable_bytes);
}