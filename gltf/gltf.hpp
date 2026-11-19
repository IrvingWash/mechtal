#pragma once

#include "gltf_types.hpp"
#include <optional>

namespace GLTF {
    std::optional<GLTF> parse_from_file(std::string_view path);
}
