#include <cstdint>
#include <iostream>

namespace detail
{
    // FNV-1a 32bit hashing algorithm.
    constexpr std::uint32_t fnv1a_32(char const* s, std::size_t count)
    {
        return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u;
    }

    consteval std::size_t constexpr_strlen(const char* str) noexcept {
        return *str ? 1 + constexpr_strlen(str + 1) : 0;
    }
}    // namespace detail

consteval std::uint32_t createHash(char const* s)
{
    return detail::fnv1a_32(s, detail::constexpr_strlen(s));
}