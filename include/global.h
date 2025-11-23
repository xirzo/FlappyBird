#ifndef FLAPPYBIRD_GLOBAL_H
#define FLAPPYBIRD_GLOBAL_H

#include <vector>
#include <random>

namespace fb {
    struct player_t {
        float x;
        float y;
        float velocity_y;
        float width;
        float height;
        float initial_x;
        float initial_y;
        float jump_force;
        float max_fall_speed;
        float max_rise_speed;
    };

    struct world_t {
        float gravity_constant;
        float wall_offset;
        float wall_gap;
        float wall_width;
        float wall_spacing;
        float wall_speed;
        float wall_unpool_x_position;
    };

    struct wall_t {
        float x;
        float y;
        bool is_hidden;
    };

    struct game_state_t {
        int window_width;
        int window_height;
        int target_fps;

        std::uniform_real_distribution<float> random_range;
        std::mt19937 random_generator;

        player_t player;
        world_t world;
        std::vector<wall_t> walls_pool;
    };
}

#endif //FLAPPYBIRD_GLOBAL_H
