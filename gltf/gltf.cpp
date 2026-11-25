#include "gltf.hpp"
#include "gltf_types.hpp"
#include "glb_types.hpp"
#include <filesystem>
#include <cstdio>
#include <optional>
#include <format>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <glaze/core/context.hpp>
#include <glaze/glaze.hpp>
#include <glaze/json/read.hpp>
#include <glaze/util/key_transformers.hpp>

static constexpr glz::opts GLTF_READ_OPTS = {
    .null_terminated       = false,
    .error_on_unknown_keys = false,
};

template <> struct glz::meta<GLTF::GLTF>                         : glz::camel_case {};
template <> struct glz::meta<GLTF::Accessor>                     : glz::camel_case {};
template <> struct glz::meta<GLTF::AccessorSparse>               : glz::camel_case {};
template <> struct glz::meta<GLTF::AccessorSparseIndices>        : glz::camel_case {};
template <> struct glz::meta<GLTF::AccessorSparseValues>         : glz::camel_case {};
template <> struct glz::meta<GLTF::Asset>                        : glz::camel_case {};
template <> struct glz::meta<GLTF::Buffer>                       : glz::camel_case {};
template <> struct glz::meta<GLTF::BufferView>                   : glz::camel_case {};
template <> struct glz::meta<GLTF::CameraPerspective>            : glz::camel_case {};
template <> struct glz::meta<GLTF::Image>                        : glz::camel_case {};
template <> struct glz::meta<GLTF::MaterialTextureInfo>          : glz::camel_case {};
template <> struct glz::meta<GLTF::MaterialPBRMetallicRoughness> : glz::camel_case {};
template <> struct glz::meta<GLTF::MaterialNormalTextureInfo>    : glz::camel_case {};
template <> struct glz::meta<GLTF::MaterialOcclusionTextureInfo> : glz::camel_case {};
template <> struct glz::meta<GLTF::Material>                     : glz::camel_case {};
template <> struct glz::meta<GLTF::Sampler>                      : glz::camel_case {};
template <> struct glz::meta<GLTF::Skin>                         : glz::camel_case {};
template <> struct glz::meta<GLTF::Node> {
    static constexpr auto modify = glz::object("matrix", &GLTF::Node::column_major_matrix);
};
template <> struct glz::meta<GLTF::MeshPrimitiveAttribute> {
    using T = GLTF::MeshPrimitiveAttribute;
    static constexpr auto value = glz::object(
        "POSITION",   &T::position,
        "NORMAL",     &T::normal,
        "TANGENT",    &T::tangent,
        "TEXCOORD_0", &T::tex_coord_0,
        "TEXCOORD_1", &T::tex_coord_1,
        "TEXCOORD_2", &T::tex_coord_2,
        "COLOR_0",    &T::color_0,
        "COLOR_1",    &T::color_1,
        "COLOR_2",    &T::color_2,
        "JOINTS_0",   &T::joints_0,
        "JOINTS_1",   &T::joints_1,
        "JOINTS_2",   &T::joints_2,
        "WEIGHTS_0",  &T::weights_0,
        "WEIGHTS_1",  &T::weights_1,
        "WEIGHTS_2",  &T::weights_2
    );
};
template <> struct glz::meta<GLTF::MeshPrimitiveMorphTarget> {
    using T = GLTF::MeshPrimitiveMorphTarget;
    static constexpr auto value = glz::object(
        "POSITION", &T::position,
        "NORMAL",   &T::normal,
        "TANGENT",  &T::tangent
    );
};
template <> struct glz::meta<GLTF::AnimationPath> {
    using enum GLTF::AnimationPath;
    static constexpr auto value = glz::enumerate(
        "translation", TRANSLATION,
        "rotation",    ROTATION,
        "scale",       SCALE,
        "weights",     WEIGHTS
    );
};
template <> struct glz::meta<GLTF::AccessorType> {
    using enum GLTF::AccessorType;
    static constexpr auto value = glz::enumerate(
        "SCALAR", SCALAR,
        "VEC2", VEC2,
        "VEC3", VEC3,
        "VEC4", VEC4,
        "MAT2", MAT2,
        "MAT3", MAT3,
        "MAT4", MAT4
    );
};
template <> struct glz::meta<GLTF::AnimationInterpolation> {
    using enum GLTF::AnimationInterpolation;
    static constexpr auto value = glz::enumerate(LINEAR, STEP, CUBICSPLINE);
};
template <> struct glz::meta<GLTF::CameraType> {
    using enum GLTF::CameraType;
    static constexpr auto value = glz::enumerate(
        "perspective",  PERSPECTIVE,
        "orthographic", ORTHOGRAPHIC
    );
};
template <> struct glz::meta<GLTF::MaterialAlphaMode> {
    using enum GLTF::MaterialAlphaMode;
    static constexpr auto value = glz::enumerate(OPAQUE, MASK, BLEND);
};

static std::nullopt_t fail(std::string *error, std::string_view message) {
    if (error) {
        *error = message;
    }

    return std::nullopt;
}

static std::optional<GLTF::GLTF> parse_gltf_from_file(const std::filesystem::path &path, std::string *error) {
    GLTF::GLTF gltf = {};

    std::string buffer;

    glz::error_ctx glz_error = glz::read_file_json<GLTF_READ_OPTS>(gltf, path.string(), buffer);
    if (glz_error.ec != glz::error_code::none) {
        return fail(
            error,
            std::format(
                "Failed to parse JSON: {}",
                glz::format_error(glz_error, buffer)
            )
        );
    }

    return gltf;
}

static std::optional<GLTF::GLTF> parse_glb_from_file(const std::filesystem::path &path, std::string *error) {
    std::error_code file_error;
    uintmax_t file_size = std::filesystem::file_size(path, file_error);
    if (file_error) {
        return fail(
            error,
            std::format(
                "Failed to get size of {}: {}",
                path.string(),
                file_error.message()
            )
        );
    }

    std::ifstream file(path,std::ios::binary);
    if (!file) {
        return fail(
            error,
            std::format(
                "Failed to open {}",
                path.string()
            )
        );
    }

    std::vector<uint8_t> file_bytes(static_cast<size_t>(file_size));
    if (
        !file.read(
            reinterpret_cast<char *>(file_bytes.data()),
            static_cast<std::streamsize>(file_size)
        )
    ) {
        return fail(
            error,
            std::format("Failed to read {}", path.string())
        );
    }

    file.close();

    size_t locator = 0;

    { // Parsing and validating the header
        GLB::Header *header = reinterpret_cast<GLB::Header*>(file_bytes.data());
        locator += sizeof(GLB::Header);

        if (header->magic != GLB::MAGIC) {
            return fail(error, "The provided file has a wrong header magic number");
        }
        if (header->version != GLB::SUPPORTED_GLB_VERSION) {
            return fail(
                error,
                std::format(
                    "The supported GLB version is {}, but the provided file has {}",
                    GLB::SUPPORTED_GLB_VERSION,
                    header->version
                )
            );
        }
        if (header->length > file_bytes.size()) {
            return fail(
                error,
                "The provided file is smaller than the length specified in its header"
            );
        }
    }

    std::string_view json = {};
    { // Parsing the first chunk header
        GLB::ChunkHeader *first_chunk_header = reinterpret_cast<GLB::ChunkHeader*>(file_bytes.data() + locator);
        locator += sizeof(GLB::ChunkHeader);

        if (first_chunk_header->type != GLB::ChunkType::JSON) {
            return fail(error, "The provided file is missing the JSON chunk");
        }
        if (first_chunk_header->length > file_bytes.size() - locator) {
            return fail(error, "The provided file's header chunk is bigger than the file itself");
        }

        json = std::string_view(
            reinterpret_cast<const char*>(file_bytes.data() + locator),
            first_chunk_header->length
        );
    }

    GLTF::GLTF gltf = {};

    glz::error_ctx glz_error = glz::read<GLTF_READ_OPTS>(gltf, json);

    if (glz_error.ec != glz::error_code::none) {
        return fail(
            error,
            std::format(
                "Failed to parse JSON: {}",
                glz::format_error(glz_error, json)
            )
        );
    }

    return gltf;
}

std::optional<GLTF::GLTF> GLTF::parse_from_file(const std::string_view &path, std::string *error) {
    std::filesystem::path fs_path = std::filesystem::path(path);

    std::filesystem::path extension = fs_path.extension();

    if (extension == ".gltf") {
        return parse_gltf_from_file(fs_path, error);
    }
    if (extension == ".glb") {
        return parse_glb_from_file(fs_path, error);
    }

    return fail(
        error,
        std::format("The provided file has an unknown extension: {}", fs_path.string())
    );
}
