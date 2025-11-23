#ifndef FLAPPYBIRD_GLOBAL_H
#define FLAPPYBIRD_GLOBAL_H

namespace fb {
    struct player_t {
        float x;
        float y;
        float velocity_y;
        float width;
        float height;
    };

    struct world_t {
        float gravity_constant;
    };

    struct game_state_t {
        int window_width;
        int window_height;
        int target_fps;

        player_t player;
        world_t world;
    };
}

#endif //FLAPPYBIRD_GLOBAL_H
