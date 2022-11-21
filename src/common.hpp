// Copyright (C) 2022 Zach Collins <zcollins4@proton.me>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <cstdint>

#ifdef _WIN32
#define LITTLE_ENDIAN 0
#define BIG_ENDIAN 1
#define BYTE_ORDER LITTLE_ENDIAN
#else
#include <endian.h>
#endif

namespace Common {

inline std::uint16_t swap16(std::uint16_t num) {
    return (num >> 8) | (num << 8);
}

inline std::uint32_t swap32(std::uint32_t num) {
    return ((num & 0xFF000000) >> 24) |
           ((num & 0x00FF0000) >> 8)  |
           ((num & 0x0000FF00) << 8)  |
           ((num & 0x000000FF) << 24);
}

inline std::uint16_t little16(std::uint16_t num) {
#if BYTE_ORDER == BIG_ENDIAN
    return swap16(num);
#else
    return num;
#endif
}

inline std::uint32_t little32(std::uint32_t num) {
#if BYTE_ORDER == BIG_ENDIAN
    return swap32(num);
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

const float PI = 3.141592653589793f;

}
