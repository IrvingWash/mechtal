#pragma once

#include <cstdint>
#include <vector>
#include <optional>
#include <string>
#include <vulkan/vulkan.h>

namespace Windowing {
    struct Window {
        void *handle = nullptr;
    };

    namespace CreateWindowFlags {
        constexpr uint64_t VULKAN    = 0x0000000010000000;
        constexpr uint64_t RESIZABLE = 0x0000000000000020;
    }

    struct DisplayInfo {
        uint32_t width  = 0;
        uint32_t height = 0;
    };

    struct Size {
        uint32_t width;
        uint32_t height;
    };

    bool init(std::string *error);
    void deinit();

    std::optional<Window> create_window(
        const char *name,
        int32_t width,
        int32_t height,
        uint64_t flags,
        std::string *error
    );
    void   destroy_window(Window &window);

    Size get_renderable_size(const Window &window);

    std::vector<uint32_t> get_display_ids(std::string *error);

    std::optional<DisplayInfo> get_display_info(uint32_t display_id, std::string *error);

    bool set_window_relative_cursor_mode(const Window &window, bool enabled, std::string *error);



    // =========================================================================
    // Vulkan-specific
    // =========================================================================

    VkSurfaceKHR create_surface_for_vulkan(
        const Window &window,
        VkInstance instance,
        VkAllocationCallbacks const *allocator = VK_NULL_HANDLE,
        std::string *error = nullptr
    );

    std::vector<const char*> get_vulkan_instance_extensions(std::string *error);
}
