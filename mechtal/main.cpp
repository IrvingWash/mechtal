#include "windowing.hpp"
#include <cassert>

class Game {
    public:
    Windowing::Window window = {0};
};

Game game;

void init() {
    Windowing::init();

    game.window = Windowing::create_window(
        "",
        800,
        600,
        Windowing::CreateWindowFlags::VULKAN | Windowing::CreateWindowFlags::RESIZABLE
    );
    assert(game.window.handle != nullptr);
}

void deinit() {
    Windowing::destroy_window(game.window);
    Windowing::deinit();
}

int main() {
    init();
    deinit();
}
