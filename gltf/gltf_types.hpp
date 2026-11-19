#pragma once

#include <limits>
#include <optional>
#include <cstdint>
#include <string>
#include <vector>
#include <array>

namespace GLTF {
    constexpr uint32_t SUPPORTED_GLTF_VERSION = 2;

    enum class AnimationInterpolation {
        LINEAR,
        STEP,
        CUBICSPLINE,
    };

    enum class AnimationPath {
        TRANSLATION,
        ROTATION,
        SCALE,
        WEIGHTS,
    };

    struct AnimationTarget {
        std::optional<uint32_t> node;
        AnimationPath           path = AnimationPath::TRANSLATION;
    };

    struct AnimationChannel {
        uint32_t        sampler = 0;
        AnimationTarget target  = {};
    };

    struct AnimationSampler {
        uint32_t               input         = 0;
        AnimationInterpolation interpolation = AnimationInterpolation::LINEAR;
        uint32_t               output        = 0;
    };

    struct Animation {
        std::vector<AnimationChannel> channels = {};
        std::vector<AnimationSampler> samplers = {};
        std::optional<std::string>    name;
    };

    struct Asset  {
        std::string copyright   = "";
        std::string generator   = "";
        std::string version     = ""; // GLTF version: <major>.<minor>
        std::string min_version = ""; // GLTF version: <major>.<minor>
    };

    struct Buffer {
        std::optional<std::string> uri;
        uint32_t                   byte_length = 0;
        std::optional<std::string> name;

        std::vector<uint8_t> bytes = {};
    };

    enum class BufferViewTarget {
        UNDEFINED            = 0,
        ARRAY_BUFFER         = 34962,
        ELEMENT_ARRAY_BUFFER = 34963,
    };

    struct BufferView {
        uint32_t                   buffer      = 0;
        uint32_t                   byte_offset = 0;
        uint32_t                   byte_length = 0;
        std::optional<uint32_t>    byte_stride;
        BufferViewTarget           target      = BufferViewTarget::UNDEFINED;
        std::optional<std::string> name;
    };

    struct CameraOrthographic {
        float xmag  = 0;
        float ymag  = 0;
        float zfar  = 0;
        float znear = 0;
    };

    struct CameraPerspective {
        std::optional<float> aspect_ratio;
        float                yfov = 0;
        float                zfar = std::numeric_limits<float>::infinity();
        float                znear = 0;
    };

    enum class CameraType {
        PERSPECTIVE,
        ORTHOGRAPHIC,
    };

    struct Camera {
        std::optional<CameraOrthographic> orthographic;
        std::optional<CameraPerspective>  perspective;
        CameraType                        type = CameraType::PERSPECTIVE;
        std::optional<std::string>        name;
    };

    struct Image {
        std::optional<std::string> uri;
        std::string                mime_type = "";
        std::optional<uint32_t>    buffer_view;
        std::optional<std::string> name;
    };

    struct MaterialTextureInfo {
        uint32_t index     = 0;
        uint32_t tex_coord = 0;
    };

    struct MaterialPBRMetallicRoughness {
        std::array<float, 4>               base_color_factor = {1, 1, 1, 1};
        std::optional<MaterialTextureInfo> base_color_texture;
        float                              metallic_factor  = 1;
        float                              roughness_factor = 1;
        std::optional<MaterialTextureInfo> metallic_roughness_texture;
    };

    struct MaterialNormalTextureInfo {
        uint32_t index     = 0;
        uint32_t tex_coord = 0;
        float    scale     = 1;
    };

    struct MaterialOcclusionTextureInfo {
        uint32_t index     = 0;
        uint32_t tex_coord = 0;
        float    strength  = 1;
    };

    enum class MaterialAlphaMode {
        OPAQUE,
        MASK,
        BLEND,
    };

    struct Material {
        MaterialPBRMetallicRoughness                pbr_metallic_roughness = {};
        std::optional<MaterialNormalTextureInfo>    normal_texture;
        std::optional<MaterialOcclusionTextureInfo> occlusion_texture;
        std::optional<MaterialTextureInfo>          emissive_texture;
        std::array<float, 3>                        emissive_factor        = {};
        MaterialAlphaMode                           alpha_mode             = MaterialAlphaMode::OPAQUE;
        float                                       alpha_cutoff           = 0.5;
        bool                                        double_sided           = false;
        std::optional<std::string>                  name;
    };

    struct MeshPrimitiveAttribute {
        std::optional<uint32_t> position;
        std::optional<uint32_t> normal;
        std::optional<uint32_t> tangent;
        std::optional<uint32_t> tex_coord_0;
        std::optional<uint32_t> tex_coord_1;
        std::optional<uint32_t> tex_coord_2;
        std::optional<uint32_t> color_0;
        std::optional<uint32_t> color_1;
        std::optional<uint32_t> color_2;
        std::optional<uint32_t> joints_0;
        std::optional<uint32_t> joints_1;
        std::optional<uint32_t> joints_2;
        std::optional<uint32_t> weights_0;
        std::optional<uint32_t> weights_1;
        std::optional<uint32_t> weights_2;
    };

    enum class MeshPrimitiveTopologyMode {
        POINTS         = 0,
        LINES          = 1,
        LINE_LOOP      = 2,
        LINE_STRIP     = 3,
        TRIANGLES      = 4,
        TRIANGLE_STRIP = 5,
        TRIANGLE_FAN   = 6,
    };

    struct MeshPrimitiveMorphTarget {
        std::optional<uint32_t> position;
        std::optional<uint32_t> normal;
        std::optional<uint32_t> tangent;
    };

    struct MeshPrimitive {
        MeshPrimitiveAttribute                attributes = {};
        std::optional<uint32_t>               indices;
        std::optional<uint32_t>               material;
        MeshPrimitiveTopologyMode             mode       = MeshPrimitiveTopologyMode::TRIANGLES;
        std::vector<MeshPrimitiveMorphTarget> targets    = {};
    };

    struct Mesh {
        std::vector<MeshPrimitive> primitives = {};
        std::vector<float>         weights    = {};
        std::optional<std::string> name;
    };

    struct Node {
        std::optional<uint32_t> camera;
        std::vector<uint32_t>   children = {};
        std::optional<uint32_t> skin;
        std::array<float, 16> column_major_matrix = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1,
        };
        std::optional<uint32_t>    mesh;
        std::array<float, 4>       rotation    = {0, 0, 0, 1};
        std::array<float, 3>       scale       = { 1, 1, 1 };
        std::array<float, 3>       translation = {};
        std::vector<float>         weights    = {};
        std::optional<std::string> name;
    };

    enum class SamplerMagFilter {
        UNDEFINED = 0,
        NEAREST   = 9728,
        LINEAR    = 9729,
    };

    enum class SamplerMinFilter {
        UNDEFINED              = 0,
        NEAREST                = 9728,
        LINEAR                 = 9729,
        NEAREST_MIPMAP_NEAREST = 9984,
        LINEAR_MIPMAP_NEAREST  = 9985,
        NEAREST_MIPMAP_LINEAR  = 9986,
        LINEAR_MIPMAP_LINEAR   = 9987,
    };

    enum class SamplerWrapMode {
        CLAMP_TO_EDGE   = 33071,
        MIRRORED_REPEAT = 33648,
        REPEAT          = 10497,
    };

    struct Sampler {
        SamplerMagFilter mag_filter = SamplerMagFilter::UNDEFINED;
        SamplerMinFilter min_filter = SamplerMinFilter::UNDEFINED;
        SamplerWrapMode  wrap_s     = SamplerWrapMode::REPEAT;
        SamplerWrapMode  wrap_t     = SamplerWrapMode::REPEAT;
    };

    struct Scene {
        std::vector<uint32_t>      nodes = {};
        std::optional<std::string> name;
    };

    struct Skin {
        std::optional<uint32_t>    inverse_bind_matrices;
        std::optional<uint32_t>    skeleton;
        std::vector<uint32_t>      joints = {};
        std::optional<std::string> name;
    };

    struct Texture {
        std::optional<uint32_t>    sampler = 0;
        std::optional<uint32_t>    source  = 0;
        std::optional<std::string> name;
    };

    struct GLTF {
        std::vector<Animation>  animations   = {};
        Asset                   asset        = {};
        std::vector<Buffer>     buffers      = {};
        std::vector<BufferView> buffer_views = {};
        std::vector<Camera>     cameras      = {};
        std::vector<Image>      images       = {};
        std::vector<Material>   materials    = {};
        std::vector<Mesh>       meshes       = {};
        std::vector<Node>       nodes        = {};
        std::vector<Sampler>    samplers     = {};
        std::optional<uint32_t> scene;
        std::vector<Scene>      scenes       = {};
        std::vector<Skin>       skins        = {};
        std::vector<Texture>   textures      = {};
    };
}
