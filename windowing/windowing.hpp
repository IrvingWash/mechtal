#pragma once

#include <cstdint>
#include <utility>
#include <vector>

namespace Windowing {
    struct Window {
        void *handle = nullptr;
    };

    namespace CreateWindowFlags {
        constexpr uint32_t VULKAN    = 0x0000000010000000;
        constexpr uint32_t RESIZABLE = 0x0000000000000020;
    }

    struct Display_Info {
        uint32_t width  = 0;
        uint32_t height = 0;
    };

    bool init();
    void deinit();

    Window create_window(const char *name, int32_t width, int32_t height, uint32_t flags);
    void   destroy_window(Window &window);

    std::pair<uint32_t, uint32_t> get_renderable_size(const Window &window);

    std::vector<uint32_t> get_display_ids();

    Display_Info get_display_info(uint32_t display_id);

    void set_window_relative_cursor_mode(const Window &window, bool enabled);
}
