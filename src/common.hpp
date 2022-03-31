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
}
