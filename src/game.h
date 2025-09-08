#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include "player.h"

//====================================================================================
// Structs and Typedefs
//====================================================================================

// GameState struct to hold the overall state of the game
typedef struct GameState
{
    Player player;
    Camera2D camera;
    Texture2D backgroundTexture;
    Font mainFont;
} GameState;

//====================================================================================
// Function Prototypes
//====================================================================================
void InitCamera(Camera2D *camera, Vector2 target);
void UpdatePlayerCamera(Camera2D *camera, const Player *player, float dt);
void RenderGame(const GameState *state);

#endif // GAME_H