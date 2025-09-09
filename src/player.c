#include "player.h"
#include <raymath.h>
#include <math.h>
#include "particle.h"
#include "config.h"

/**
 * @brief Initializes the player with default values.
 */
void InitPlayer(Player *player, Texture2D texture)
{
    player->texture = texture;
    player->textureRect = (Rectangle){0, 0, 16, 16};
    player->position = (Vector2){1280 / 2.0f, 720 / 2.0f};
    player->velocity = (Vector2){0, 0};

    float aspectRatio = (float)player->textureRect.width / player->textureRect.height;
    player->width = 75;
    player->height = player->width / aspectRatio;
    player->origin = (Vector2){player->width / 2.0f, player->height / 2.0f};

    // --- SET PLAYER VARIABLES ---
    player->forwardSpeed = 0.0f;
    player->rotation = 0.0f;
    player->maxSpeed = 500.0f;
    player->maxRotationSpeed = 125.0f;
    player->acceleration = 600.0f;
    player->deceleration = 500.0f;
    player->friction = 0.4f;
    player->gripNormal = 9.0f;
    player->gripDrifting = 0.0f;
    player->isDrifting = false;
    player->isMoving = false;
    player->driftTurnMultiplier = 3.0f;

    // --- SET PARTICLE VARIABLES
    player->skidParticleSize = 10.0f;
    float widthFactor = player->width / 5.5f;
    float heightFactor = player->height / 5.5f;

    // 0: Front-Left
    player->wheelOffsets[0] = (Vector2){-widthFactor, -heightFactor};
    // 1: Front-Right
    player->wheelOffsets[1] = (Vector2){widthFactor, -heightFactor};
    // 2: Rear-Left
    player->wheelOffsets[2] = (Vector2){-widthFactor, heightFactor};
    // 3: Rear-Right
    player->wheelOffsets[3] = (Vector2){widthFactor, heightFactor};
}

/**
 * @brief Handles user input. Now controls acceleration force and rotation.
 */
void HandleInput(Player *player, float dt)
{
    player->isDrifting = IsKeyDown(KEY_SPACE);

    // Acceleration and Braking (influences forwardSpeed)
    if (IsKeyDown(KEY_UP))
    {
        player->forwardSpeed += player->acceleration * dt;
        if (player->forwardSpeed > player->maxSpeed)
            player->forwardSpeed = player->maxSpeed;
    }
    else if (IsKeyDown(KEY_DOWN))
    {
        player->forwardSpeed -= player->deceleration * dt;
        if (player->forwardSpeed < -player->maxSpeed / 2.0f)
            player->forwardSpeed = -player->maxSpeed / 2.0f;
    }
    else // If no input, engine thrust returns to zero
    {
        if (player->forwardSpeed > 0)
        {
            player->forwardSpeed -= player->acceleration / 2 * dt;
            if (player->forwardSpeed < 0)
                player->forwardSpeed = 0;
        }
        else if (player->forwardSpeed < 0)
        {
            player->forwardSpeed += player->acceleration / 2 * dt;
            if (player->forwardSpeed > 0)
                player->forwardSpeed = 0;
        }
    }

    player->isMoving = (Vector2Length(player->velocity) > 10.0f);

    // Rotation
    if (player->isMoving)
    {
        float currentRotationSpeed = player->forwardSpeed / player->maxSpeed * player->maxRotationSpeed;
        if (player->isDrifting)
        {
            currentRotationSpeed *= player->driftTurnMultiplier;
        }

        if (IsKeyDown(KEY_RIGHT))
        {
            player->rotation += currentRotationSpeed * dt;
        }
        else if (IsKeyDown(KEY_LEFT))
        {
            player->rotation -= currentRotationSpeed * dt;
        }
    }
}

/**
 * @brief Updates player position using momentum, grip, and drift physics.
 */
void UpdatePlayer(Player *player, ParticleSystem *system, float dt)
{
    if (player->isDrifting && player->isMoving)
    {
        // Emit a particle at the player's current position and rotation
        float radians = player->rotation * DEG2RAD;
        for (int i = 2; i < 4; i++)
        {
            // Rotate the wheel's local offset by the car's current rotation
            Vector2 rotatedOffset = Vector2Rotate(player->wheelOffsets[i], radians);

            // Add the rotated offset to the car's world position to get the wheel's world position
            Vector2 wheelWorldPos = Vector2Add(player->position, rotatedOffset);

            // Emit a particle from that position
            EmitSkidParticle(system, wheelWorldPos, player->rotation, player->skidParticleSize);
        }
    }

    // Wrap rotation value
    if (player->rotation >= 360.0f)
        player->rotation -= 360.0f;
    if (player->rotation < 0.0f)
        player->rotation += 360.0f;

    float radians = player->rotation * DEG2RAD;
    Vector2 forwardVec = {sinf(radians), -cosf(radians)};
    Vector2 targetVelocity = Vector2Scale(forwardVec, player->forwardSpeed);
    float currentGrip = player->isDrifting ? player->gripDrifting : player->gripNormal;
    player->velocity = Vector2Lerp(player->velocity, targetVelocity, currentGrip * dt);
    player->velocity = Vector2Scale(player->velocity, 1.0f - (player->friction * dt));

    Vector2 newPosition = Vector2Add(player->position, Vector2Scale(player->velocity, dt));

    // --- BOUNDARY CHECKS ---
    // Define the boundaries
    float minX = player->width / 2.5f;
    float maxX = WORLD_WIDTH - (player->width / 2.5f);
    float minY = player->height / 2.5f;
    float maxY = WORLD_HEIGHT - (player->height / 2.5f);

    // Check and clamp X-axis
    if (newPosition.x < minX)
    {
        newPosition.x = minX;
        player->velocity.x = 0;
    }
    if (newPosition.x > maxX)
    {
        newPosition.x = maxX;
        player->velocity.x = 0; 
    }

    // Check and clamp Y-axis
    if (newPosition.y < minY)
    {
        newPosition.y = minY;
        player->velocity.y = 0; 
    }
    if (newPosition.y > maxY)
    {
        newPosition.y = maxY;
        player->velocity.y = 0;
    }
    player->position = newPosition;
}