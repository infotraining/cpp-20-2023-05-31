#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std::literals;

TEST_CASE("safe comparing integral numbers")
{
    int x = -7;
    unsigned int y = 665;
    // CHECK(x < y); // ERROR
    CHECK(std::cmp_less(x, y));

    auto my_cmp_less = [](std::integral auto a, std::integral auto b) {
        return std::cmp_less(a, b);
    };

    CHECK(my_cmp_less(x, y));

    // CHECK(my_cmp_less(1, "1")); // ERROR - "1" is not integral type

    CHECK(std::in_range<std::size_t>(-1) == false);
    CHECK(std::in_range<std::size_t>(42) == true);
}

///////////////////////////////////////////////////////////////////

struct Value
{
    int value;

    friend std::ostream& operator<<(std::ostream& out, const Value& v)
    {
        out << "Value{" << v.value << "}";
        return out;
    }

    Value operator-() const
    {
        return Value{-value};
    }

    //bool operator==(const Value& other) const = default; // - implicitly defaulted when <=> is default

    auto operator<=>(const Value& other) const = default;

    // [[nodiscard]] friend constexpr bool operator==(const Value& lhs, const Value& rhs) noexcept
    // {
    //     return lhs.value == rhs.value;
    // }

    // [[nodiscard]] friend constexpr bool operator!=(const Value& lhs, const Value& rhs) noexcept
    // {
    //     return !(lhs.value == rhs.value);
    // }
};

Value operator""_v(unsigned long long int value)
{
    return Value{static_cast<int>(value)};
}

struct Person
{
    std::string name;
    int age;
    std::string nick;

    //bool operator==(const Person&) const = default; // all members compared - implicitly defaulted when <=> is default
    
    auto operator<=>(const Person&) const = default;

    // bool operator==(const Person& other) const // custom impl - name & age compared
    // {
    //     return std::tie(name, age) == std::tie(other.name, other.age);
    // }

    bool operator<(const Person& other) const
    {
        // if (name == other.name)
        // {
        //     if (age == other.age)
        //     {
        //         return nick < othe.nick;    
        //     }
            
        //     return age < other.age;
        // }
        // return name < other.name;

        std::cout << "Person::op<()\n";

        return std::tie(name, age, nick) < std::tie(other.name, other.age, other.nick); // OK since C++11
    }
};

struct Human
{
    std::string name;
    int how_old;
    double height;

    bool operator==(const Person& other) const
    {
        return std::tie(name, how_old) == std::tie(other.name, other.age);
    }

    //bool operator==(const Person&) const = default;

    bool operator==(const Human& other) const
    {
        return std::tie(name, how_old) == std::tie(other.name, other.how_old);
    }

    std::strong_ordering operator<=>(const Human& other) const 
    {
        if (auto cmp_result = name <=> other.name; cmp_result == 0)
        {
            return how_old <=> other.how_old;
        }
        else
            return cmp_result;
    }
};

TEST_CASE("Value - operator ==")
{
    Value v1{42};
    auto v2 = 42_v;

    REQUIRE(v1 == v2);
    REQUIRE(v1 != 665_v); // !(v1 == 665_v) - rewriting expression
}

TEST_CASE("Value - ordering")
{
    std::vector data = { 42_v, 1_v, -7_v, 665_v, 0_v, 42_v };
    
    std::sort(data.begin(), data.end());

    CHECK(Value{42} <=> 42_v == std::strong_ordering::equal);
    CHECK(Value{42} == 42_v);
    CHECK(Value{42} <= 42_v); //    
}

TEST_CASE("Person - operator ==")
{
    CHECK(Person{"John", 23, "J"} == Person{"John", 23, "J"});
    CHECK(Person{"John", 23, "J"} != Person{"John", 24, "J"});

    CHECK(Person{"John", 23, "J"} == Human{"John", 23});

    CHECK(Person{"John", 23, "J"} < Person{"John", 24, "J"});
    CHECK(Person{"John", 25, "J"} > Person{"John", 24, "J"});
}

TEST_CASE("operator<=>")
{
    int x = 42;

    CHECK(x <=> 42 == 0); // x == 42
    CHECK(x <=> 665 < 0); // x < 665
    CHECK(x <=> 22 > 0);  // x > 22

    SECTION("strong ordering")
    {
        std::strong_ordering result = 55 <=> 77;

        CHECK(result == std::strong_ordering::less);
        CHECK(77 <=> 77 == std::strong_ordering::equal);
        CHECK(665 <=> 77 == std::strong_ordering::greater);
    }

    SECTION("partial ordering")
    {
        CHECK(3.0 <=> 3.14 == std::partial_ordering::less);
        CHECK(3.14 <=> 3.14 == std::partial_ordering::equivalent);
        CHECK(0.0 <=> -0.0 == std::partial_ordering::equivalent);
        CHECK(std::numeric_limits<double>::quiet_NaN() <=> 5.66 == std::partial_ordering::unordered);
        CHECK(std::numeric_limits<double>::quiet_NaN() <=> 5.66 == std::partial_ordering::unordered);
        CHECK(std::numeric_limits<double>::quiet_NaN() <=> std::numeric_limits<double>::quiet_NaN() == std::partial_ordering::unordered);
    }   
}

///////////////////////////////////////////////
// custom operator<=>

TEST_CASE("Human - <=>")
{
    Human h1{"Eva", 33, 167.6};
    Human h2{"Eva", 32, 167.6};

    CHECK(h1 <=> h2 == std::partial_ordering::greater);
    CHECK(h1 == h2);
}

struct IntNan
{
    std::optional<int> value = std::nullopt;

public:
    bool operator==(const IntNan& rhs) const
    {
        if (!value || !rhs.value)
        {
            return false;
        }
        return *value == *rhs.value;
    }

    std::partial_ordering operator<=>(const IntNan& rhs) const
    {
        if (!value || !rhs.value)
            return std::partial_ordering::unordered;

        return *value <=> *rhs.value; // std::strong_ordering is implicitly converted to std::partial_ordering
    }
};

TEST_CASE("IntNan")
{
    CHECK(IntNan{2} <=> IntNan{4} == std::partial_ordering::less);
    CHECK(IntNan{2} <=> IntNan{} == std::partial_ordering::unordered);

    CHECK(IntNan{2} < IntNan{4});       // true
    CHECK_FALSE(IntNan{2} < IntNan{});  // false
    CHECK_FALSE(IntNan{2} == IntNan{}); // false
    CHECK_FALSE(IntNan{2} <= IntNan{}); // false
}