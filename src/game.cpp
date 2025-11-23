#include "game.h"

#include <raylib.h>
#include <chrono>

#include "aabb.h"
#include "render.h"

namespace fb {
    constexpr Color BACKGROUND_COLOR = {78, 178, 206, 255};

    void add_wall(game_state_t &state, float x_offset) {
        float x = static_cast<float>(state.window_width) -
                  static_cast<float>(state.window_width) / 4
                  + x_offset;
        float random_y = state.random_range(state.random_generator);
        state.walls_pool.emplace_back(x, random_y, false);
    }

    void init(game_state_t &state) {
        SetTraceLogLevel(LOG_ERROR);
        InitWindow(state.window_width, state.window_height, "Flappy Bird");
        SetTargetFPS(state.target_fps);

        state.player.x = state.player.initial_x;
        state.player.y = state.player.initial_y;

        long long seed = std::chrono::system_clock::now()
                .time_since_epoch()
                .count();

        state.random_generator = std::mt19937(seed);

        float top_random_y_position =
                state.world.wall_offset +
                state.world.wall_gap / 2.0f;

        float bottom_random_y_position =
                static_cast<float>(state.window_height) -
                state.world.wall_offset -
                state.world.wall_gap / 2.0f;

        state.random_range = std::uniform_real_distribution(top_random_y_position, bottom_random_y_position);

        add_wall(state, 0);
        add_wall(state, 200);
        add_wall(state, 400);

        state.world.wall_unpool_x_position = 600;
    }

    void restart(game_state_t &state) {
        state.player.x = state.player.initial_x;
        state.player.y = state.player.initial_y;
        state.player.velocity_y = 0;

        state.walls_pool.clear();

        add_wall(state, 0);
        add_wall(state, 200);
        add_wall(state, 400);
    }

    void player_jump(game_state_t &state) {
        state.player.velocity_y = -state.player.jump_force;
    }

    bool should_close() {
        return WindowShouldClose();
    }

    void process_input(game_state_t &state) {
        if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            player_jump(state);
        }
    }

    void draw(const game_state_t &state) {
        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);
        draw_player(state);
        draw_walls(state);
        EndDrawing();
    }

    void move_player(game_state_t &state) {
        const float deltaTime = GetFrameTime();
        state.player.velocity_y += state.world.gravity_constant * deltaTime;

        if (state.player.velocity_y > state.player.max_fall_speed) {
            state.player.velocity_y = state.player.max_fall_speed;
        }
        if (state.player.velocity_y < state.player.max_rise_speed) {
            state.player.velocity_y = state.player.max_rise_speed;
        }

        state.player.y += state.player.velocity_y * deltaTime;
    }

    void clamp_player_position(game_state_t &state) {
        if (state.player.y >= static_cast<float>(state.window_height) - state.player.height ||
            state.player.y <= 0) {
            restart(state);
        }
    }

    void move_walls(game_state_t &state) {
        const float deltaTime = GetFrameTime();

        for (wall_t &wall: state.walls_pool) {
            wall.x -= state.world.wall_speed * deltaTime;
        }
    }

    void pool_walls_if_out_of_screen(game_state_t &state) {
        for (wall_t &wall: state.walls_pool) {
            if (wall.is_hidden || wall.x > -state.world.wall_width) {
                continue;
            }

            wall.is_hidden = true;
        }
    }

    void unpool_walls_if_pooled(game_state_t &state) {
        for (wall_t &wall: state.walls_pool) {
            if (wall.is_hidden == false) {
                continue;
            }

            float random_y = state.random_range(state.random_generator);

            wall.x = state.world.wall_unpool_x_position;
            wall.y = random_y;
            wall.is_hidden = false;
        }
    }


    void restart_if_colliding(game_state_t &state) {
        for (wall_t &wall: state.walls_pool) {
            if (are_player_wall_colliding(state.player, wall, state)) {
                restart(state);
            }
        }
    }

    void update(game_state_t &state) {
        move_player(state);
        move_walls(state);
        pool_walls_if_out_of_screen(state);
        unpool_walls_if_pooled(state);
        restart_if_colliding(state);
        clamp_player_position(state);
    }

    void deinit() {
        CloseWindow();
    }
}
