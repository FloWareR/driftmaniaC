#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include "player.h"
#include "particle.h"
#include "entity.h"

//====================================================================================
// Structs and Typedefs
//====================================================================================
struct AudioManager;

// Enum holds all the possible game "screens"
typedef enum GameScreen
{
    SPLASH_SCREEN = 0,
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
    Texture2D carTexture;
    Texture2D logoTexture;
    Texture2D mainMenuBackground; 
    Font mainFont;
    ParticleSystem particleSystem;
    GameScreen currentScreen;
    EntityManager entityManager;
    struct AudioManager *audioManager;
    int timer;
    bool isDebugMode;
} GameState;

//====================================================================================
// Function Prototypes
//====================================================================================
void InitCamera(Camera2D *camera, Vector2 target);
void UpdatePlayerCamera(Camera2D *camera, const Player *player, float dt);
void LoadLevel(EntityManager *em);
void RenderGame(const GameState *state);
void ResetGameplayState(GameState *state);

#endif // GAME_H