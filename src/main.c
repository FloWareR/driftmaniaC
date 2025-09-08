#include <raylib.h>
#include "config.h"
#include "player.h"
#include "game.h"

//====================================================================================
// Main Entry Point
//====================================================================================
int main(void)
{
    // Initialize Window 
    //----------------------------------------------------------------------------------
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Drift Mania C");
    SetTargetFPS(60);

    GameState state = {0};

    // Load Textures
    Texture2D car_texture = LoadTexture("src/assets/Sprites/Cars/Player_red (16 x 16).png");
    state.backgroundTexture = LoadTexture("src/assets/Sprites/Levels/Soil_Tile.png");

    // Load Font
    state.mainFont = LoadFont("src/assets/Fonts/PixelFont.ttf");
    
    // Initialize Game Objects
    InitPlayer(&state.player, car_texture);
    InitCamera(&state.camera, state.player.position);
    //----------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        //------------------------------------------------------------------------------
        float dt = GetFrameTime();

        HandleInput(&state.player, dt);
        UpdatePlayer(&state.player, dt);
        UpdatePlayerCamera(&state.camera, &state.player, dt);
        //------------------------------------------------------------------------------

        // Draw
        //------------------------------------------------------------------------------
        RenderGame(&state);
        //------------------------------------------------------------------------------
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