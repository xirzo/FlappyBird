#include <iostream>

#include "game.h"
#include "render.h"

int main() {
    fb::game_state_t state = {0};
    state.window_width = 720 / 2;
    state.window_height = 1280 / 2;
    state.target_fps = 60;

    auto ww = static_cast<float>(state.window_width);
    auto wh = static_cast<float>(state.window_height);

    state.world.wall_width = ww * 0.167f;
    state.world.wall_gap = wh * 0.234f;
    state.world.wall_offset = wh * 0.02f;
    state.world.wall_spacing = ww * 0.58f;
    state.world.wall_speed = ww * 0.28f;

    state.world.wall_unpool_x_position = ww + state.world.wall_width;

    state.world.gravity_constant = wh * 0.94f;
    state.player.jump_force = wh * 0.78f;
    state.player.max_fall_speed = wh * 0.625f;
    state.player.max_rise_speed = -wh * 0.46875f;

    state.player.initial_x = ww / 5.0f;
    state.player.initial_y = wh / 2.0f;
    state.player.height = wh * 0.08f;
    state.player.width = state.player.height;

    fb::init(state);

    auto renderer_init_result = fb::init_renderer();

    if (!renderer_init_result) {
        std::cerr << "ERROR: " << renderer_init_result.error() << std::endl;
        return EXIT_FAILURE;
    }

    while (fb::should_close() == false) {
        fb::process_input(state);
        fb::draw(state);
        fb::update(state);
    }

    fb::deinit();
    return EXIT_SUCCESS;
}
