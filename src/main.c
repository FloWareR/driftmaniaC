#include <raylib.h>
#include <stdlib.h>
#include "config.h"
#include "player.h"
#include "game.h"
#include "particle.h"
#include "ui.h"
#include "audio.h"

//====================================================================================
// Main Entry Point
//====================================================================================
int main(void)
{
    // Initialize Window
    //----------------------------------------------------------------------------------
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_NAME);
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    GameState state = {0};

    state.audioManager = (AudioManager *)malloc(sizeof(AudioManager));
    if (state.audioManager == NULL)
        return 1;

    InitAudioManager(state.audioManager);
    state.currentScreen = SPLASH_SCREEN;

    // Load Textures
    state.carTexture = LoadTexture("src/assets/Sprites/Cars/car-red.png");
    state.mainMenuBackground = LoadTexture("src/assets/main_menu_background.jpg");
    state.backgroundTexture = LoadTexture("src/assets/Sprites/Levels/Soil_Tile.png");
    state.logoTexture = LoadTexture("src/assets/flowarelogo.png");

    // Load Font
    state.mainFont = LoadFont("src/assets/Fonts/PixelFont.ttf");

    // Initialize Game Objects
    ResetGameplayState(&state);
    //----------------------------------------------------------------------------------

    PlayGameMusic(state.audioManager, SPLASH_SCREEN);

    // Main game loop
    while (state.currentScreen != QUIT && !WindowShouldClose())
    {
        UpdateGameMusic(state.audioManager);

        if (IsKeyPressed(KEY_F3))
            state.isDebugMode = !state.isDebugMode;

        // --- UPDATE LOGIC based on the current screen ---
        switch (state.currentScreen)
        {
        case SPLASH_SCREEN:
            UpdateSplashScreen(&state);
            break;
        case MAIN_MENU:
            UpdateMainMenu(&state);
            break;
        case GAMEPLAY:
            if (IsKeyPressed(PAUSE_KEY))
            {
                state.currentScreen = PAUSE_MENU;
                break;
            }
            float dt = GetFrameTime();
            CheckPlayerEntityCollisions(&state.player, &state.entityManager);
            HandleInput(&state.player, dt);
            UpdatePlayer(&state.player, &state.particleSystem, dt);
            UpdatePlayerCamera(&state.camera, &state.player, dt);
            UpdateParticleSystem(&state.particleSystem, dt);
            break;
        case PAUSE_MENU:
            UpdatePauseMenu(&state);
            break;
        default:
            break;
        }

        // --- Render LOGIC based on the current screen ---
        BeginDrawing();

        switch (state.currentScreen)
        {
        case SPLASH_SCREEN:
            RenderplashScreen(&state);
            break;
        case MAIN_MENU:
            RenderMainMenu(&state);
            break;
        case GAMEPLAY:
            RenderGame(&state);
            break;
        case PAUSE_MENU:
            RenderPauseMenu(&state);
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
    UnloadTexture(state.logoTexture);
    UnloadAudioManager(state.audioManager);
    UnloadTexture(state.mainMenuBackground); 
    CloseWindow();
    //----------------------------------------------------------------------------------

    return 0;
}