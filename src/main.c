#include <raylib.h>
#include "config.h"
#include "player.h"
#include "game.h"
#include "particle.h"
#include "ui.h"


//====================================================================================
// Main Entry Point
//====================================================================================
int main(void)
{
    // Initialize Window
    //----------------------------------------------------------------------------------
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_NAME);
    SetTargetFPS(60);

    GameState state = {0};
    state.currentScreen = MAIN_MENU;

    // Load Textures
    Texture2D car_texture = LoadTexture("src/assets/Sprites/Cars/Player_red (16 x 16).png");
    state.backgroundTexture = LoadTexture("src/assets/Sprites/Levels/Soil_Tile.png");

    // Load Font
    state.mainFont = LoadFont("src/assets/Fonts/PixelFont.ttf");

    // Initialize Game Objects
    InitPlayer(&state.player, car_texture);
    InitCamera(&state.camera, state.player.position);
    InitParticleSystem(&state.particleSystem);
    //----------------------------------------------------------------------------------

    // Main game loop
    while (state.currentScreen != QUIT && !WindowShouldClose())
    {
        // --- UPDATE LOGIC based on the current screen ---
        switch (state.currentScreen)
        {
        case MAIN_MENU:
            UpdateMainMenu(&state);
            break;
        case GAMEPLAY:
        {
            float dt = GetFrameTime();
            HandleInput(&state.player, dt);
            UpdatePlayer(&state.player, &state.particleSystem, dt);
            UpdatePlayerCamera(&state.camera, &state.player, dt);
            UpdateParticleSystem(&state.particleSystem, dt);
        }
        break;
        default:
            break;
        }

        // --- DRAW LOGIC based on the current screen ---
        BeginDrawing();

        switch (state.currentScreen)
        {
        case MAIN_MENU:
            RenderMainMenu(&state);
            break;
        case GAMEPLAY:
            RenderGame(&state);
            break;
        default:
            break;
        }

        EndDrawing();
    }

    // Cleanup
    //----------------------------------------------------------------------------------
    UnloadTexture(state.player.texture);
    UnloadTexture(state.backgroundTexture);
    UnloadFont(state.mainFont);
    CloseWindow();
    //----------------------------------------------------------------------------------

    return 0;
}