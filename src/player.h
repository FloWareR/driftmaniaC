#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

//====================================================================================
// Structs and Typedefs
//====================================================================================
struct ParticleSystem;

// Player struct
typedef struct Player
{
    int score;
    Vector2 position;
    Vector2 origin;
    Vector2 velocity;
    Texture2D texture;
    Rectangle textureRect;
    float width;
    float height;
    float rotation;
    bool isDrifting;
    bool isMoving;

    float forwardSpeed;
    float maxSpeed;
    float acceleration;
    float deceleration;
    float friction;
    float gripNormal;
    float gripDrifting;
    float maxRotationSpeed;
    float driftTurnMultiplier;

    // Particle settings
    Vector2 wheelOffsets[4];
    float skidParticleSize;
} Player;

//====================================================================================
// Function Prototypes
//====================================================================================
void InitPlayer(Player *player, Texture2D texture);
void HandleInput(Player *player, float dt);
void UpdatePlayer(Player *player, struct ParticleSystem *system, float dt);
#endif // PLAYER_H