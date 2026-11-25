#include "windowing.hpp"
#include "input.hpp"
#include "gltf.hpp"
#include <cassert>
#include <optional>

class Game {
    public:
    Windowing::Window window      = {};
    Input::State      input_state = {};

    bool should_quit = false;
};

Game game;

void init() {
    bool ok = Windowing::init();
    assert(ok);

    std::optional<Windowing::Window> window = Windowing::create_window(
        "",
        800,
        600,
        Windowing::CreateWindowFlags::VULKAN | Windowing::CreateWindowFlags::RESIZABLE
    );
    assert(window.has_value());
    game.window = window.value();
}

void deinit() {
    Windowing::destroy_window(game.window);
    Windowing::deinit();
}

int main() {
    init();

    while (!game.should_quit) {
        Input::update(game.input_state);

        for (const Input::WindowEvent &event: game.input_state.window_events) {
            switch (event.kind) {
                case Input::WindowEventKind::QUIT:
                    game.should_quit = true;
                    break;
            }
        }
    }

    deinit();
}
