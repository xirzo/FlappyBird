#include "game.h"

int main() {
    fb::game_state_t state = {0};
    state.window_width = 720 / 2;
    state.window_height = 1280 / 2;
    state.target_fps = 60;

    state.world.gravity_constant = 600;

    state.player.initial_x = static_cast<float>(state.window_width) / 5;
    state.player.initial_y = static_cast<float>(state.window_height) / 2;

    state.player.width = 60;
    state.player.height = 60;

    state.player.jump_force = 500.0;
    state.player.max_fall_speed = 400.0;
    state.player.max_rise_speed = -300.0;

    fb::init(state);

    while (fb::should_close() == false) {
        fb::process_input(state);
        fb::draw(state);
        fb::update(state);
    }

    fb::deinit();
    return 0;
}
