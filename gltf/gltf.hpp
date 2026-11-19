#pragma once

#include "gltf_types.hpp"
#include <optional>

namespace GLTF {
    std::optional<GLTF> parse_from_file(const char *path);
}
