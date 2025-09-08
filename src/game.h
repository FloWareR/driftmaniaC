#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include "player.h"
#include "particle.h"

//====================================================================================
// Structs and Typedefs
//====================================================================================

// Enum holds all the possible game "screens"
typedef enum GameScreen {
    MAIN_MENU,
    GAMEPLAY,
    PAUSE_MENU,
    QUIT
} GameScreen;

// GameState struct to hold the overall state of the game
typedef struct GameState
{
    Player player;
    Camera2D camera;
    Texture2D backgroundTexture;
    Font mainFont;
    ParticleSystem particleSystem;
    GameScreen currentScreen;
} GameState;


//====================================================================================
// Function Prototypes
//====================================================================================
void InitCamera(Camera2D *camera, Vector2 target);
void UpdatePlayerCamera(Camera2D *camera, const Player *player, float dt);
void RenderGame(const GameState *state);

#endif // GAME_H