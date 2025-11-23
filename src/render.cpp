#include "render.h"

#include "raylib.h"

namespace fb {
    void draw_player(const game_state_t &state) {
        DrawRectangle(state.player.x, state.player.y, state.player.width, state.player.height, WHITE);
    }
}
