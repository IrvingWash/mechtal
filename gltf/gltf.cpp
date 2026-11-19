#include "gltf.hpp"
#include "gltf_types.hpp"
#include <filesystem>
#include <cstdio>
#include <glaze/glaze.hpp>
#include <glaze/util/key_transformers.hpp>
#include <optional>

static constexpr glz::opts GLTF_READ_OPTS = { .error_on_unknown_keys = false };

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
template <>
struct glz::meta<GLTF::Node> {
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

static std::optional<GLTF::GLTF> parse_gltf_from_file(std::filesystem::path path) {
    GLTF::GLTF gltf = {};
    
    std::string buffer;

    glz::error_ctx error = glz::read_file_json<GLTF_READ_OPTS>(gltf, path.string(), buffer);
    if (error.ec != glz::error_code::none) {
        std::fprintf(
            stderr,
            "Failed to parse JSON: %s\n",
            glz::format_error(error, buffer).c_str()
        );

        return std::nullopt;
    }

    return gltf;
}

std::optional<GLTF::GLTF> GLTF::parse_from_file(std::string_view path) {
    std::filesystem::path fs_path = std::filesystem::path(path);

    std::filesystem::path extension = fs_path.extension();

    std::optional<GLTF> gltf = std::nullopt;
    if (extension == ".gltf") {
        gltf = parse_gltf_from_file(fs_path);
    } else if (extension == ".glb") {
        // @Todo
    } else {
        std::fprintf(stderr, "The provided file has an unknown extension: %s\n", fs_path.string().c_str());
    }

    return gltf;
}
