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

void Input::State::update() {
    if (!this->mouse_state.has_position) {
        SDL_GetMouseState(
            &this->mouse_state.cursor_position.first,
            &this->mouse_state.cursor_position.second
        );
        this->mouse_state.has_position = true;
    }

    this->window_events.clear();

    for (uint8_t &key_state : this->key_states) {
        key_state &= ~(Input::KeyState::PRESSED | Input::KeyState::RELEASED);
    }

    this->mouse_state.left_button_state   &= ~(Input::KeyState::PRESSED | Input::KeyState::RELEASED);
    this->mouse_state.right_button_state  &= ~(Input::KeyState::PRESSED | Input::KeyState::RELEASED);
    this->mouse_state.middle_button_state &= ~(Input::KeyState::PRESSED | Input::KeyState::RELEASED);

    this->mouse_state.cursor_delta = {};
    this->mouse_state.wheel_delta  = {};

    SDL_Event sdl_event;

    while (SDL_PollEvent(&sdl_event)) {
        switch (sdl_event.type) {
            case SDL_EVENT_QUIT: {
                Input::WindowEvent event = {
                    .kind = Input::WindowEventKind::QUIT,
                };

                this->window_events.push_back(event);
                break;
            }
            case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: {
                Input::WindowEvent event = {
                    .kind = Input::WindowEventKind::WINDOW_PIXEL_SIZE_CHANGED,
                    .window_pixel_size_changed = {
                        .new_width = uint32_t(sdl_event.window.data1),
                        .new_height = uint32_t(sdl_event.window.data2),
                    },
                };
                this->window_events.push_back(event);
                break;
            }
            case SDL_EVENT_MOUSE_MOTION: {
                this->mouse_state.cursor_position = {sdl_event.motion.x, sdl_event.motion.y};
                this->mouse_state.cursor_delta.first += sdl_event.motion.xrel;
                this->mouse_state.cursor_delta.second += sdl_event.motion.yrel;
                break;
            }
            case SDL_EVENT_KEY_DOWN: {
                if (!sdl_event.key.repeat) {
                    uint8_t &key_state = this->key_states[sdl_event.key.scancode];
                    key_state |= Input::KeyState::DOWN | Input::KeyState::PRESSED;
                }
                break;
            }
            case SDL_EVENT_KEY_UP: {
                uint8_t &key_state = this->key_states[sdl_event.key.scancode];
                key_state &= ~Input::KeyState::DOWN;
                key_state |= Input::KeyState::RELEASED;
                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_DOWN: {
                uint8_t *button = get_button_state_from_sdl_button(this->mouse_state, sdl_event.button.button);
                if (button) {
                    *button |= Input::KeyState::DOWN | Input::KeyState::PRESSED;
                }
                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_UP: {
                uint8_t *button = get_button_state_from_sdl_button(this->mouse_state, sdl_event.button.button);
                if (button) {
                    *button &= ~Input::KeyState::DOWN;
                    *button |= Input::KeyState::RELEASED;
                }
                break;
            }
            case SDL_EVENT_MOUSE_WHEEL: {
                this->mouse_state.wheel_delta.first += sdl_event.wheel.x;
                this->mouse_state.wheel_delta.second += sdl_event.wheel.y;

                break;
            }
        }
    }
}
