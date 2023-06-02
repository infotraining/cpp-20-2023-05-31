module; // global fragment module

#include <cmath>
#include <cstdint>
#include <limits>
#include <array>

export module Primes; // declare module Primes

export constexpr bool is_prime(uint32_t n)
{
    bool is_prime = true;

    for (auto i = 2u; i <= std::sqrt(n); ++i)
    {
        if (n % i == 0)
            return false;
    }

    return true;
}

export struct IsPrime
{
    bool operator()(uint32_t n) const
    {
        return is_prime(n);
    }
};

export template <uint32_t N>
constexpr std::array<uint32_t, N> get_primes()
{
    std::array<uint32_t, N> primes{};
    
    uint32_t n = 0;
    for (uint32_t i = 2; n < N; ++i)
        if (is_prime(i))
        {
            primes[n++] = i;
        }

    return primes;
}

export template <uint32_t N>
constexpr auto first_primes = get_primes<10>();

export const std::array first_100_primes = get_primes<100>();