#include "render.h"

#include "raylib.h"
#include <cmath>

namespace fb {
    static Texture2D player_texture;
    static Texture2D tunnel_line_texture;
    static Texture2D tunnel_end_texture;

    static Texture2D background_texture;
    static Texture2D midground_texture;
    static Texture2D foreground_texture;

    constexpr char PLAYER_IMAGE_PATH[] = "assets/duck.png";
    constexpr char TUNNEL_LINE_IMAGE_PATH[] = "assets/tunnel_line.png";
    constexpr char TUNNEL_END_IMAGE_PATH[] = "assets/tunnel_end.png";
    constexpr char BACKGROUND_IMAGE_PATH[] = "assets/background.png";
    constexpr char MIDGROUND_IMAGE_PATH[] = "assets/midground.png";
    constexpr char FOREGROUND_IMAGE_PATH[] = "assets/foreground.png";
    constexpr float TUNNEL_END_OFFSET = -3.0f;

    constexpr float PARALLAX_INTENSITY = 1.6f;
    constexpr float BACKGROUND_PARALLAX = 0.06f * PARALLAX_INTENSITY;
    constexpr float MIDGROUND_PARALLAX = 0.40f * PARALLAX_INTENSITY;
    constexpr float FOREGROUND_PARALLAX = 1.10f * PARALLAX_INTENSITY;

    constexpr float SWAY_FREQ = 0.9f;
    constexpr float SWAY_AMPLITUDE = 6.0f;

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

        if (FileExists(BACKGROUND_IMAGE_PATH) == false) {
            return std::unexpected("Background image is not found");
        }

        Image background_image = LoadImage(BACKGROUND_IMAGE_PATH);
        background_texture = LoadTextureFromImage(background_image);
        UnloadImage(background_image);
        SetTextureFilter(background_texture, TEXTURE_FILTER_POINT);

        if (FileExists(MIDGROUND_IMAGE_PATH) == false) {
            return std::unexpected("Midground image is not found");
        }

        Image midground_image = LoadImage(MIDGROUND_IMAGE_PATH);
        midground_texture = LoadTextureFromImage(midground_image);
        UnloadImage(midground_image);
        SetTextureFilter(midground_texture, TEXTURE_FILTER_POINT);

        if (FileExists(FOREGROUND_IMAGE_PATH) == false) {
            return std::unexpected("Foreground image is not found");
        }

        Image foreground_image = LoadImage(FOREGROUND_IMAGE_PATH);
        foreground_texture = LoadTextureFromImage(foreground_image);
        UnloadImage(foreground_image);
        SetTextureFilter(foreground_texture, TEXTURE_FILTER_POINT);

        return {};
    }

    void deinit_renderer() {
        UnloadTexture(player_texture);
        UnloadTexture(tunnel_line_texture);
        UnloadTexture(tunnel_end_texture);
        UnloadTexture(background_texture);
        UnloadTexture(midground_texture);
        UnloadTexture(foreground_texture);
    }

    void draw_background(const game_state_t &state) {
        auto draw_layer = [&](const Texture2D &tex, float parallax_factor, Color tint, float extra_scale = 1.0f) {
            if (tex.id == 0 || tex.width == 0 || tex.height == 0) return;

            auto target_height = static_cast<float>(state.window_height);
            float base_scale = target_height / static_cast<float>(tex.height);
            float scale = base_scale * extra_scale;
            float tile_width = static_cast<float>(tex.width) * scale;

            float raw_offset = -state.player.x * parallax_factor;
            double offset_d = std::fmod(static_cast<double>(raw_offset), static_cast<double>(tile_width));
            auto offset = static_cast<float>(offset_d);

            if (offset > 0) {
                offset -= tile_width;
            }

            double sway_d = std::sin(GetTime() * SWAY_FREQ) * SWAY_AMPLITUDE * (parallax_factor / FOREGROUND_PARALLAX);
            auto sway = static_cast<float>(sway_d);

            auto screen_w = GetScreenWidth();
            int tiles = static_cast<int>(std::ceil((static_cast<float>(screen_w) - offset) / tile_width)) + 3;

            for (int i = 0; i < tiles; ++i) {
                float x = offset + static_cast<float>(i) * tile_width;
                DrawTextureEx(tex, {x, sway}, 0.0f, scale, tint);
            }
        };

        draw_layer(background_texture, BACKGROUND_PARALLAX, Fade(WHITE, 0.75f), 1.0f);
        draw_layer(midground_texture, MIDGROUND_PARALLAX, Fade(WHITE, 0.9f), 1.01f);
        draw_layer(foreground_texture, FOREGROUND_PARALLAX, Fade(WHITE, 1.0f), 1.04f);
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
