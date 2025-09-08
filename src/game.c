#include "game.h"
#include "config.h"
#include <raymath.h>

/**
 * @brief Initializes the camera with default values.
 */
void InitCamera(Camera2D *camera, Vector2 target)
{
    camera->target = target;
    camera->offset = (Vector2){SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
    camera->rotation = 0.0f;
    camera->zoom = 1.0f;
}

/**
 * @brief Updates the camera. Now uses the player's velocity for look-ahead.
 */
void UpdatePlayerCamera(Camera2D *camera, const Player *player, float dt)
{
    // Calculate a point ahead of the car for the camera to look at, based on its actual velocity
    Vector2 lookAheadPoint = Vector2Add(player->position, Vector2Scale(player->velocity, CAMERA_LOOK_AHEAD_FACTOR));

    // Smoothly interpolate the camera's target towards the desired target (Lerp)
    camera->target.x += (lookAheadPoint.x - camera->target.x) * CAMERA_SMOOTHNESS * dt;
    camera->target.y += (lookAheadPoint.y - camera->target.y) * CAMERA_SMOOTHNESS * dt;

    // Clamp camera to world boundaries
    float min_x = SCREEN_WIDTH / 2.0f;
    float max_x = WORLD_WIDTH - (SCREEN_WIDTH / 2.0f);
    float min_y = SCREEN_HEIGHT / 2.0f;
    float max_y = WORLD_HEIGHT - (SCREEN_HEIGHT / 2.0f);

    camera->target.x = CLAMP(camera->target.x, min_x, max_x);
    camera->target.y = CLAMP(camera->target.y, min_y, max_y);
}

/**
 * @brief Renders the entire game scene.
 */
void RenderGame(const GameState *state)
{
    ClearBackground(DARKGRAY);
    BeginMode2D(state->camera);

    // Draw tiled background
    int tile_count_col = WORLD_WIDTH / state->backgroundTexture.width;
    int tile_count_row = WORLD_HEIGHT / state->backgroundTexture.height;
    for (int y = 0; y < tile_count_row; y++)
    {
        for (int x = 0; x < tile_count_col; x++)
        {
            DrawTexture(state->backgroundTexture, x * state->backgroundTexture.width, y * state->backgroundTexture.height, WHITE);
        }
    }

    // Draw particles
    DrawParticleSystem(&state->particleSystem);

    // Draw Player
    const Player *player = &state->player;
    Rectangle car_dest_rect = {player->position.x, player->position.y, player->width, player->height};
    DrawTexturePro(
        player->texture,
        player->textureRect,
        car_dest_rect,
        player->origin,
        player->rotation,
        player->isDrifting ? YELLOW : WHITE);

    EndMode2D();

    // UI elements
    DrawFPS(SCREEN_WIDTH - 90, 10);
    DrawTextEx(state->mainFont, "UP: Accelerate\nDOWN: Brake/Reverse\nSPACE: Drift", (Vector2){10, 10}, 24, 1, WHITE);
    DrawTextEx(state->mainFont, TextFormat("SPEED: %.0f", Vector2Length(player->velocity)), (Vector2){10, 90}, 24, 1, WHITE);
}