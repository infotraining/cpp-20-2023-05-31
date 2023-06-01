#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <format>
#include <compare>
#include <cmath>

using namespace std::literals;

struct Rating
{
    int value;

    bool operator==(const Rating& other) const
    {
        return value == other.value;
    }

    bool operator<(const Rating& other) const
    {
        return value < other.value;
    }
};

struct Gadget
{
    std::string name;
    double price;

    bool operator==(const Gadget& other) const = default;

    std::strong_ordering operator<=>(const Gadget& other) const 
    {
        if (std::strong_ordering cmp_result = name <=> other.name; cmp_result != 0)
        {
            return cmp_result;
        }

        return std::strong_order(price, other.price);
    }
};

struct SuperGadget : Gadget
{
    Rating rating;

    std::strong_ordering operator<=>(const SuperGadget& other) const
    {
        if (auto cmp_result = (*this).Gadget::operator<=>(other); cmp_result != 0)
            return cmp_result;
        return std::compare_strong_order_fallback(rating, other.rating);
    }

    bool operator==(const SuperGadget& other) const = default;
};

TEST_CASE("Gadget - write custom operator <=> - stronger category than auto detected")
{
    SECTION("==")
    {
        CHECK(Gadget{"ipad", 1.0} == Gadget{"ipad", 1.0});
    }
    
    SECTION("<=>")
    {
        static_assert(std::is_same_v<
                decltype(std::declval<Gadget>() <=> std::declval<Gadget>()), 
                std::strong_ordering
            >);
        
        CHECK(Gadget{"ipad", 1.0} <=> Gadget{"ipad", 1.0} == std::strong_ordering::equal);
    }
}

TEST_CASE("SuperGadget - write custom operator <=> - member without compare-three-way operator")
{
    CHECK(SuperGadget{{"ipad", 1.0}, Rating{1}} != SuperGadget{{"ipad", 1.0}, Rating{2}});
    CHECK(SuperGadget{{"ipad", 1.0}, Rating{1}} <=> SuperGadget{{"ipad", 1.0}, Rating{2}} == std::strong_ordering::less);    
    CHECK(SuperGadget{{"ipad", 2.0}, Rating{1}} <=> SuperGadget{{"ipad", 1.0}, Rating{2}} == std::strong_ordering::greater);    
}

//////////////////////////////////////////////////////////////////////////////////////////////////////


enum class RatingValue : uint8_t { very_poor = 1, poor, satisfactory, good, very_good, excellent};

struct RatingStar
{
public:
    RatingValue value;

    explicit RatingStar(RatingValue rating_value) : value{rating_value}
    {} 

    auto operator<=>(const RatingStar&) const = default;

    // auto operator<=>(RatingValue rv) const
    // {
    //     return value <=> rv;
    // }
};

auto operator<=>(RatingStar rs, RatingValue rv)
{
    return rs.value <=> rv;
}

TEST_CASE("Rating Star - implement needed <=>")
{
    RatingStar r1{RatingValue::good};
    
    CHECK(r1 == RatingStar{RatingValue::good});
    CHECK(r1 <=> RatingStar{RatingValue::excellent} == std::strong_ordering::less);
    CHECK(r1 <=> RatingValue::excellent == std::strong_ordering::less);
}