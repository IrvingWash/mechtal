#include "windowing.hpp"
#include <optional>
#include <string>
#include <format>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

bool Windowing::init(std::string *error) {
    bool ok = SDL_InitSubSystem(SDL_INIT_VIDEO);
    if (!ok && error) {
        *error = std::format("Failed to initialize windowing: {}", SDL_GetError());
    }

    return ok;
}

void Windowing::deinit() {
    SDL_QuitSubSystem(SDL_INIT_VIDEO);

    if (SDL_WasInit(0) == 0) {
        SDL_SetHint(SDL_HINT_SHUTDOWN_DBUS_ON_QUIT, "1");
        SDL_Quit();
    }
}

std::optional<Windowing::Window> Windowing::create_window(
    const char *name,
    int32_t width,
    int32_t height,
    uint64_t flags,
    std::string *error
) {
    SDL_Window *sdl_window = SDL_CreateWindow(name, width, height, flags);
    if (!sdl_window) {
        if (error ) {
            *error = std::format("Failed to create window: {}", SDL_GetError());
        }
        return std::nullopt;
    }

    return Window{ .handle = sdl_window };
}

void Windowing::destroy_window(Window &window) {
    SDL_DestroyWindow(static_cast<SDL_Window*>(window.handle));

    window.handle = nullptr;
}

Windowing::Size Windowing::get_renderable_size(const Window &window) {
    int width  = 0;
    int height = 0;
    SDL_GetWindowSizeInPixels(
        static_cast<SDL_Window*>(window.handle),
        &width,
        &height
    );

    Size size = {
        .width = uint32_t(width),
        .height = uint32_t(height),
    };

    return size;
}

std::vector<uint32_t> Windowing::get_display_ids(std::string *error) {
    int32_t count = 0;
    SDL_DisplayID *sdl_display_ids = SDL_GetDisplays(&count);
    if (!sdl_display_ids) {
        if (error) {
            *error = std::format("Failed to get display ids: {}", SDL_GetError());
        }
        return {};
    }

    std::vector<uint32_t> result(sdl_display_ids, sdl_display_ids + count);

    SDL_free(sdl_display_ids);

    return result;
}

std::optional<Windowing::DisplayInfo> Windowing::get_display_info(uint32_t display_id, std::string *error) {
    const SDL_DisplayMode *sdl_display_mode = SDL_GetDesktopDisplayMode(display_id);
    if (!sdl_display_mode) {
        if (error) {
            *error = std::format("Failed to get display mode: {}", SDL_GetError());
        }
        return std::nullopt;
    }

    Windowing::DisplayInfo result = {
        .width  = static_cast<uint32_t>(sdl_display_mode->w),
        .height = static_cast<uint32_t>(sdl_display_mode->h),
    };

    return result;
}

bool Windowing::set_window_relative_cursor_mode(const Window &window, bool enabled, std::string *error) {
    bool ok = SDL_SetWindowRelativeMouseMode(
        static_cast<SDL_Window*>(window.handle),
        enabled
    );

    if (!ok && error) {
        *error = std::format("Failed to set window relative cursor mode: {}", SDL_GetError());
    }

    return ok;
}

VkSurfaceKHR Windowing::create_surface_for_vulkan(
    const Window &window,
    VkInstance instance,
    const VkAllocationCallbacks *allocator,
    std::string *error
) {
    VkSurfaceKHR surface = VK_NULL_HANDLE;

    bool ok = SDL_Vulkan_CreateSurface(
        static_cast<SDL_Window*>(window.handle),
        instance,
        allocator,
        &surface
    );
    if (!ok && error) {
        *error = std::format("Failed to create surface: {}", SDL_GetError());
    }

    return surface;
}

std::vector<const char*> Windowing::get_vulkan_instance_extensions(std::string *error) {
    Uint32 count = 0;
    auto raw_extensions = SDL_Vulkan_GetInstanceExtensions(&count);
    if (!raw_extensions) {
        if (error) {
            *error = std::format("Failed to get Vulkan instance extensions: {}", SDL_GetError());
        }
        return {};
    }

    return std::vector(raw_extensions, raw_extensions + count);
}

static_assert(Windowing::CreateWindowFlags::VULKAN    == SDL_WINDOW_VULKAN);
static_assert(Windowing::CreateWindowFlags::RESIZABLE == SDL_WINDOW_RESIZABLE);
