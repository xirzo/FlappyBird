#include "render.h"

#include "raylib.h"

namespace fb {
    static Texture2D player_texture;
    static Texture2D tunnel_line_texture;
    static Texture2D tunnel_end_texture;

    constexpr char PLAYER_IMAGE_PATH[] = "assets/duck.png";
    constexpr char TUNNEL_LINE_IMAGE_PATH[] = "assets/tunnel_line.png";
    constexpr char TUNNEL_END_IMAGE_PATH[] = "assets/tunnel_end.png";
    constexpr float TUNNEL_END_OFFSET = -3.0f;

    std::expected<void, std::string> init_renderer() {
        if (FileExists(PLAYER_IMAGE_PATH) == false) {
            return std::unexpected("Player image is not found");
        }

        Image player_image = LoadImage(PLAYER_IMAGE_PATH);
        player_texture = LoadTextureFromImage(player_image);
        UnloadImage(player_image);
        SetTextureFilter(player_texture, TEXTURE_FILTER_POINT);

        if (FileExists(TUNNEL_LINE_IMAGE_PATH) == false) {
            return std::unexpected("Tunnel line image is not found");
        }

        Image tunnel_line_image = LoadImage(TUNNEL_LINE_IMAGE_PATH);
        tunnel_line_texture = LoadTextureFromImage(tunnel_line_image);
        UnloadImage(tunnel_line_image);
        SetTextureFilter(tunnel_line_texture, TEXTURE_FILTER_POINT);

        if (FileExists(TUNNEL_END_IMAGE_PATH) == false) {
            return std::unexpected("Tunnel end image is not found");
        }

        Image tunnel_end_image = LoadImage(TUNNEL_END_IMAGE_PATH);
        tunnel_end_texture = LoadTextureFromImage(tunnel_end_image);
        UnloadImage(tunnel_end_image);
        SetTextureFilter(tunnel_end_texture, TEXTURE_FILTER_POINT);
        return {};
    }

    void deinit_renderer() {
        UnloadTexture(player_texture);
        UnloadTexture(tunnel_line_texture);
        UnloadTexture(tunnel_end_texture);
    }

    void draw_player(const game_state_t &state) {
        float scale_x = state.player.width / static_cast<float>(player_texture.width);
        float scale_y = state.player.height / static_cast<float>(player_texture.height);
        float scale = (scale_x < scale_y) ? scale_x : scale_y;

        DrawTextureEx(player_texture, {state.player.x, state.player.y}, 0.0f, scale, WHITE);
#ifdef DEBUG_MODE
        DrawSphere({state.player.x, state.player.y, 8}, 10, GREEN);
#endif
    }

    void draw_walls(const game_state_t &state) {
        for (const wall_t &wall: state.walls_pool) {
#ifdef DEBUG_MODE
            DrawSphere({wall.x, wall.y, 8}, 10, GREEN);
#endif

            if (wall.is_hidden) {
                continue;
            }

            float scale = state.world.wall_width / static_cast<float>(tunnel_line_texture.width);
            float line_height = static_cast<float>(tunnel_line_texture.height) * scale;
            float end_height = static_cast<float>(tunnel_end_texture.height) * scale;

            float top_wall_bottom_y = wall.y - state.world.wall_gap / 2.0f;
            float current_y = top_wall_bottom_y + end_height;

            if (current_y + end_height > 0) {
                DrawTextureEx(tunnel_end_texture, {wall.x + TUNNEL_END_OFFSET, current_y}, 0.0f, scale, WHITE);
            }

            current_y -= line_height;
            while (current_y + line_height > 0) {
                DrawTextureEx(tunnel_line_texture, {wall.x, current_y}, 0.0f, scale, WHITE);
                current_y -= line_height;
            }

            float bottom_wall_top_y = wall.y + state.world.wall_gap / 2.0f;

            current_y = bottom_wall_top_y - end_height;
            if (current_y < static_cast<float>(state.window_height)) {
                DrawTextureEx(tunnel_end_texture, {wall.x + TUNNEL_END_OFFSET, current_y}, 0.0f, scale, WHITE);
            }

            current_y += end_height;
            while (current_y < static_cast<float>(state.window_height)) {
                DrawTextureEx(tunnel_line_texture, {wall.x, current_y}, 0.0f, scale, WHITE);
                current_y += line_height;
            }
        }
    }
}
