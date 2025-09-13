#ifndef ENTITY_H
#define ENTITY_H

#include <raylib.h>
#include "player.h"

#define MAX_ENTITIES 128

//====================================================================================
// Structs and Typedefs
//====================================================================================
typedef enum EntityType
{
    COLLECTABLE_SCORE,
    HAZARD_CAR
} EntityType;

typedef enum MovementPattern
{
    PATTERN_STATIC,
    PATTERN_STRAIGHT_LINE,
} MovementPattern;

typedef struct Entity
{
    Vector2 position;
    Texture2D texture;
    Vector2 velocity;
    float speed;
    Rectangle bounds;
    EntityType type;
    bool isActive;
    Color color;
    MovementPattern pattern;
} Entity;

typedef struct EntityManager
{
    Entity entities[MAX_ENTITIES];
} EntityManager;

typedef struct SpawnConfig
{
    EntityManager *em;
    EntityType type;
    int count;
} SpawnConfig;

//====================================================================================
// Function Prototypes
//====================================================================================
void InitEntityManager(EntityManager *em);
void UpdateEntities(EntityManager *em, float dt);
void DrawEntities(const EntityManager *em, bool isDebug);
void SpawnEntity(EntityManager *em, EntityType type, Vector2 position);
void CheckPlayerEntityCollisions(Player *player, EntityManager *em);
void SpawnEntitiesRandomly(SpawnConfig config);

#endif