#pragma once

#include <cstdint>

namespace Common {

inline std::uint32_t little32(std::uint32_t num) {
#if BYTE_ORDER == BIG_ENDIAN
    return (num >> 24) | (num >> 8) | (num << 8) | (num << 24);
#else
    return num;
#endif
}

/**
 * Finds the sign of a number
 * @param value The number to check
 * @return -1, 0, or +1 depending on the sign of the number
 */ 
template <typename T> int sign(const T &value) {
    return (T(0) < value) - (value < T(0));
}

}
