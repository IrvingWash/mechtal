#pragma once

#include "gltf_types.hpp"
#include <optional>

namespace GLTF {
    std::optional<GLTF> parse_from_file(const std::string_view &path, std::string *error);
}
