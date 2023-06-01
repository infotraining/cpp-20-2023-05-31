#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <string>
#include <vector>

using namespace std::literals;

bool is_even(int x)
{
    return x % 2 == 0;
}

using T = int;

template <typename T>
struct Identity
{
    using type = T;
};

template <typename T>
using Identity_t = typename Identity<T>::type;

static_assert(std::is_same_v<Identity_t<int>, int>);

//////////////////////////////////////////////
// IsVoid

template <typename T>
struct IsVoid
{
    constexpr static bool value = false;
};

template <>
struct IsVoid<void>
{
    constexpr static bool value = true;
};

template <typename T>
constexpr bool IsVoid_v = IsVoid<T>::value;

static_assert(IsVoid_v<T> == false);
static_assert(IsVoid_v<void> == true);

////////////////////////////////////////////
// IsPointer

template <typename T>
struct IsPointer
{
    static constexpr bool value = false;
};

template <typename T>
struct IsPointer<T*>
{
    static constexpr bool value = true;
};

template <typename T>
constexpr bool IsPointer_v = IsPointer<T>::value;

using U = int;
using V = int*;

static_assert(IsPointer_v<U> == false);
static_assert(IsPointer_v<V> == true);

/////////////////////////////////////////////
//

template <typename T>
struct IsContigues
{
    static constexpr bool value = false;
};

template <typename T>
struct IsContigues<std::vector<T>>
{
    static constexpr bool value = true;
};

template <typename T, size_t N>
struct IsContigues<T[N]>
{
    static constexpr bool value = true;
};

template <typename T>
constexpr bool IsContigues_v = IsContigues<T>::value;

static_assert(IsContigues_v<std::vector<int>>);
static_assert(IsContigues_v<std::map<int, int>> == false);
static_assert(IsContigues_v<int[10]>);

////////////////////////////////////////////////

template <bool B, class T = void>
struct EnableIf
{ };

template <class T>
struct EnableIf<true, T>
{
    using type = T;
};

template <bool Condition, typename T = void>
using EnableIf_t = typename EnableIf<Condition, T>::type;

namespace BeforeCpp20
{
    template <typename T, typename = typename std::enable_if<!IsPointer_v<T>>::type>
    T max_value(T a, T b)
    {
        // static_assert(!IsPointer_v<T>);
        return a < b ? b : a;
    }

    template <typename T, typename = void, typename = EnableIf_t<IsPointer_v<T>>>
    auto max_value(T a, T b)
    {
        // static_assert(!IsPointer_v<T>);
        return *a < *b ? *b : *a;
    }
} // namespace BeforeCpp20

namespace Cpp20
{
    template <typename T>
    concept Pointer = IsPointer_v<T>;

    static_assert(Pointer<int> == false);
    static_assert(Pointer<int*> == true);
    static_assert(Pointer<const int*> == true);

    namespace ver_1
    {
        template <typename T>
        T max_value(T a, T b)
            requires(!IsPointer_v<T>)
        {
            return a < b ? b : a;
        }

        template <typename T>
        auto max_value(T a, T b)
            requires IsPointer_v<T>
        {
            return *a < *b ? *b : *a;
        }

        auto foo(auto arg)
            requires((sizeof(arg) < 8) && (sizeof(arg) > 4))
        {
            std::cout << "foo(" << arg << ")\n";
        }
    } // namespace ver_1

    namespace ver_2
    {

        template <typename T>
        T max_value(T a, T b)
            requires(!Pointer<T>)
        {
            return a < b ? b : a;
        }

        template <Pointer T>
        auto max_value(T a, T b)
        {
            assert(a != nullptr);
            assert(b != nullptr);

            return max_value(*a, *b);
        }
    } // namespace ver_2

    namespace ver_3
    {
        template <typename T>
        T max_value(T a, T b)
            requires(!Pointer<T> && std::three_way_comparable<T>)
        {
            return a < b ? b : a;
        }

        std::integral auto max_value(Pointer auto a, Pointer auto b)
            requires std::three_way_comparable_with<decltype(*a), decltype(*b)>
        {
            assert(a != nullptr);
            assert(b != nullptr);

            return max_value(*a, *b);
        }
    } // namespace ver_3

    inline namespace ver_4
    {
        template <typename T>
        concept PointerLike = requires(T ptr) {
            *ptr;
            ptr == nullptr;
            ptr != nullptr;
        };

        template <typename T>
        T max_value(T a, T b)
            requires(!PointerLike<T> && std::three_way_comparable<T>)
        {
            return a < b ? b : a;
        }

        template <PointerLike T>
        auto max_value(T a, T b)
            requires std::three_way_comparable_with<decltype(*a), decltype(*b)>
        {
            assert(a != nullptr);
            assert(b != nullptr);
            return *a < *b ? *b : *a;
        }
    } // namespace ver_4
} // namespace Cpp20

TEST_CASE("constraints")
{
    using namespace Cpp20;

    int x = 7;
    int y = 42;

    CHECK(max_value(x, y) == 42);

    std::integral auto result = max_value(&x, &y);
    CHECK(result == 42);

    auto sp1 = std::make_shared<int>(42);
    auto sp2 = std::make_shared<int>(665);

    CHECK(ver_4::max_value(sp1, sp2) == 665);
    // foo(std::vector<int>{})
    // foo('a');
}

template <typename T>
concept BigType = requires(T obj) {
    requires sizeof(obj) > 8;
};

static_assert(BigType<int> == false);
static_assert(BigType<std::vector<int>>);

///////////////////////////////////////////////////////////

namespace Cpp11_14
{

    namespace Detail
    {
        template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
        bool is_power_of_2_impl(T value)
        {
            return value > 0 && (value & (value - 1)) == 0;
        }

        template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
        auto is_power_of_2_impl(T value)
        {
            int exponent;
            const T mantissa = std::frexp(value, &exponent);
            return mantissa == static_cast<T>(0.5);
        }
    }

    template <typename T>
    bool is_power_of_2(T value)
    {
        return Detail::is_power_of_2_impl(value);
    }


} // namespace Cpp11_14

bool is_power_of_2(std::integral auto value)
{
    return value > 0 && (value & (value - 1)) == 0;
}

auto is_power_of_2(std::floating_point auto value)
{
    int exponent;
    const T mantissa = std::frexp(value, &exponent);
    return mantissa == static_cast<T>(0.5);
}

namespace Cpp17
{
    template <typename T>
    bool is_power_of_2(T value)
    {
        if constexpr (std::is_integral_v<T>)
        {
            return value > 0 && (value & (value - 1)) == 0;
        }
        else
        {
            int exponent;
            const T mantissa = std::frexp(value, &exponent);
            return mantissa == static_cast<T>(0.5);
        }
    }

} // namespace Cpp17

TEST_CASE("is_power_of_2")
{
    REQUIRE(is_power_of_2(4));
    REQUIRE(is_power_of_2(8));
    REQUIRE(is_power_of_2(32));
    REQUIRE(is_power_of_2(77) == false);

    REQUIRE(is_power_of_2(8.0));
}

//////////////////////////////////////////

auto add_item(auto& container, auto item)
{
    if constexpr (requires { container.push_back(item); })
    {
        container.push_back(item);
        return 42;
    }
    else
    {
        container.insert(item);
        return "42"s;
    }
}

TEST_CASE("adding items to container")
{
    std::vector<int> vec;

    add_item(vec, 1);
    add_item(vec, 2);
    add_item(vec, 3);

    CHECK(vec == std::vector{1, 2, 3});
}

void print_item(std::integral auto item)
{
    std::cout << "print_item(std::integral auto item): " << item << "\n";
}

void print_item(std::integral auto item)
    requires requires { std::cout << item; }
{
    std::cout << "print_item(std::integral auto item) rr: " << item << "\n";
}

TEST_CASE("printing items")
{
    print_item(6);
}