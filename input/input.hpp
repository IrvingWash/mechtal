#pragma once

#include "scancode.hpp"
#include <cstdint>
#include <vector>
#include <array>

namespace Input {
    enum class WindowEventKind : uint8_t {
        NONE,
        QUIT,
        WINDOW_PIXEL_SIZE_CHANGED,
    };

    struct Window_Pixel_Size_Changed_Event {
        uint32_t new_width  = 0;
        uint32_t new_height = 0;
    };

    struct WindowEvent {
        WindowEventKind kind = WindowEventKind::NONE;

        union {
            bool                            quit                      = false;
            Window_Pixel_Size_Changed_Event window_pixel_size_changed;
        };
    };

    struct Position {
        float x;
        float y;
    };

    namespace KeyState {
        constexpr uint8_t DOWN     = 1 << 0;
        constexpr uint8_t PRESSED  = 1 << 1;
        constexpr uint8_t RELEASED = 1 << 2;
    }

    struct MouseState {
        Position cursor_position = {};
        Position cursor_delta    = {};
        Position wheel_delta     = {};

        uint8_t left_button_state   = 0;
        uint8_t right_button_state  = 0;
        uint8_t middle_button_state = 0;

        bool has_position;
    };

    struct State {
        std::vector<WindowEvent> window_events;

        MouseState mouse_state = {};
        std::array<uint8_t, size_t(Scancode::COUNT)> key_states = {};
    };

    void update(State &state);
}
