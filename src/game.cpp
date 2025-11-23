#include "game.h"

#include <raylib.h>

#include "render.h"

namespace fb {
    void init(const game_state_t &state) {
        SetTraceLogLevel(LOG_ERROR);
        InitWindow(state.window_width, state.window_height, "Flappy Bird");
        SetTargetFPS(state.target_fps);
    }

    bool should_close() {
        return WindowShouldClose();
    }

    void process_input() {
    }

    void draw(const game_state_t &state) {
        BeginDrawing();
        ClearBackground(BLACK);
        draw_player(state);
        EndDrawing();
    }

    void update(game_state_t &state) {
        float deltaTime = GetFrameTime();

        state.player.velocity_y += state.world.gravity_constant * deltaTime;
        state.player.y += state.player.velocity_y * deltaTime;
    }

    void deinit() {
        CloseWindow();
    }
}
