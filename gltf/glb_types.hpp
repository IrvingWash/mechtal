#pragma once

#include <cstdint>

namespace GLB {
    constexpr uint32_t MAGIC                 = 0x46546C67;
    constexpr uint32_t SUPPORTED_GLB_VERSION = 2;

    struct Header {
        uint32_t magic   = 0;
        uint32_t version = 0;
        uint32_t length  = 0;
    };

    enum class ChunkType : uint32_t {
        JSON = 0x4E4F534A,
        BIN  = 0x004E4942,
    };

    struct ChunkHeader {
        uint32_t length = 0;
        ChunkType type  = ChunkType::JSON;
    };

    static_assert(sizeof(Header) == 12);
    static_assert(sizeof(ChunkHeader) == 8);
}
