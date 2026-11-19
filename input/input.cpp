#include "input.hpp"
#include <SDL3/SDL.h>

static uint8_t *get_button_state_from_sdl_button(Input::MouseState &mouse_state, uint8_t sdl_button) {
    switch (sdl_button) {
        case SDL_BUTTON_LEFT: {
            return &mouse_state.left_button_state;
        }
        case SDL_BUTTON_RIGHT: {
            return &mouse_state.right_button_state;
        }
        case SDL_BUTTON_MIDDLE: {
            return &mouse_state.middle_button_state;
        }
        default:
            return nullptr;
    }
}

void Input::update(Input::State &state) {
    if (!state.mouse_state.has_position) {
        SDL_GetMouseState(
            &state.mouse_state.cursor_position.x,
            &state.mouse_state.cursor_position.y
        );
        state.mouse_state.has_position = true;
    }

    state.window_events.clear();

    for (uint8_t &key_state : state.key_states) {
        key_state &= ~(KeyState::PRESSED | KeyState::RELEASED);
    }

    state.mouse_state.left_button_state   &= ~(KeyState::PRESSED | KeyState::RELEASED);
    state.mouse_state.right_button_state  &= ~(KeyState::PRESSED | KeyState::RELEASED);
    state.mouse_state.middle_button_state &= ~(KeyState::PRESSED | KeyState::RELEASED);

    state.mouse_state.cursor_delta = {};
    state.mouse_state.wheel_delta  = {};

    SDL_Event sdl_event;

    while (SDL_PollEvent(&sdl_event)) {
        switch (sdl_event.type) {
            case SDL_EVENT_QUIT: {
                WindowEvent event = {
                    .kind = WindowEventKind::QUIT,
                };

                state.window_events.push_back(event);
                break;
            }
            case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: {
                WindowEvent event = {
                    .kind = WindowEventKind::WINDOW_PIXEL_SIZE_CHANGED,
                    .window_pixel_size_changed = {
                        .new_width = uint32_t(sdl_event.window.data1),
                        .new_height = uint32_t(sdl_event.window.data2),
                    },
                };
                state.window_events.push_back(event);
                break;
            }
            case SDL_EVENT_MOUSE_MOTION: {
                MouseState &mouse_state = state.mouse_state;
                mouse_state.cursor_position = {sdl_event.motion.x, sdl_event.motion.y};
                mouse_state.cursor_delta.x += sdl_event.motion.xrel;
                mouse_state.cursor_delta.y += sdl_event.motion.yrel;
                break;
            }
            case SDL_EVENT_KEY_DOWN: {
                if (!sdl_event.key.repeat) {
                    uint8_t &key_state = state.key_states[sdl_event.key.scancode];
                    key_state |= KeyState::DOWN | KeyState::PRESSED;
                }
                break;
            }
            case SDL_EVENT_KEY_UP: {
                uint8_t &key_state = state.key_states[sdl_event.key.scancode];
                key_state &= ~KeyState::DOWN;
                key_state |= KeyState::RELEASED;
                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_DOWN: {
                uint8_t *button = get_button_state_from_sdl_button(state.mouse_state, sdl_event.button.button);
                if (button) {
                    *button |= KeyState::DOWN | KeyState::PRESSED;
                }
                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_UP: {
                uint8_t *button = get_button_state_from_sdl_button(state.mouse_state, sdl_event.button.button);
                if (button) {
                    *button &= ~KeyState::DOWN;
                    *button |= KeyState::RELEASED;
                }
                break;
            }
            case SDL_EVENT_MOUSE_WHEEL: {
                MouseState &mouse_state = state.mouse_state;
                mouse_state.wheel_delta.x += sdl_event.wheel.x;
                mouse_state.wheel_delta.y += sdl_event.wheel.y;

                break;
            }
        }
    }
}
