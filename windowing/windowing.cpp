#include <cstdio>
#include <SDL3/SDL.h>
#include "windowing.hpp"
#include "SDL3/SDL_mouse.h"

bool Windowing::init() {
    bool ok = SDL_InitSubSystem(SDL_INIT_VIDEO);
    if (!ok) {
        std::fprintf(stderr, "Failed to initialize windowing: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void Windowing::deinit() {
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

Windowing::Window Windowing::create_window(
    const char *name,
    int32_t width,
    int32_t height,
    uint32_t flags
) {
    SDL_Window *sdl_window = SDL_CreateWindow(name, width, height, flags);
    if (!sdl_window) {
        std::fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
    }

    return Window{ .handle = sdl_window };
}

void Windowing::destroy_window(Window &window) {
    SDL_DestroyWindow(static_cast<SDL_Window*>(window.handle));

    window.handle = nullptr;
}

std::pair<uint32_t, uint32_t> Windowing::get_renderable_size(const Window &window) {
    int32_t width, height;
    SDL_GetWindowSizeInPixels(
        static_cast<SDL_Window*>(window.handle),
        &width,
        &height
    );

    return std::pair(width, height);
}

std::vector<uint32_t> Windowing::get_display_ids() {
    int32_t count;
    SDL_DisplayID *sdl_display_ids = SDL_GetDisplays(&count);

    std::vector<uint32_t> result(sdl_display_ids, sdl_display_ids + count);

    SDL_free(sdl_display_ids);

    return result;
}

Windowing::Display_Info get_display_info(uint32_t display_id) {
    const SDL_DisplayMode *sdl_display_mode = SDL_GetDesktopDisplayMode(display_id);

    Windowing::Display_Info result = {
        .width  = static_cast<uint32_t>(sdl_display_mode->w),
        .height = static_cast<uint32_t>(sdl_display_mode->h),
        .dpi    = sdl_display_mode->pixel_density,
    };

    return result;
}

void Windowing::set_window_relative_cursor_mode(const Window &window, bool enabled) {
    bool ok = SDL_SetWindowRelativeMouseMode(
        static_cast<SDL_Window*>(window.handle),
        enabled
    );

    if (!ok) {
        std::fprintf(stderr, "Failed to set window relative cursor mode windowing: %s\n", SDL_GetError());
    }
}

static_assert(Windowing::CreateWindowFlags::VULKAN    == SDL_WINDOW_VULKAN);
static_assert(Windowing::CreateWindowFlags::RESIZABLE == SDL_WINDOW_RESIZABLE);
