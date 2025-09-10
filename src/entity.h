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
    OBSTACLE_ROCK
} EntityType;

typedef struct Entity
{
    Vector2 position;
    Texture2D texture;
    Rectangle bounds;
    EntityType type;
    bool isActive;
    Color color;
} Entity;

typedef struct EntityManager
{
    Entity entities[MAX_ENTITIES];
} EntityManager;

//====================================================================================
// Function Prototypes
//====================================================================================
void InitEntityManager(EntityManager *em);
void DrawEntities(const EntityManager *em, bool isDebug);
void SpawnEntity(EntityManager *em, EntityType type, Vector2 position);
void CheckPlayerEntityCollisions(Player *player, EntityManager *em);

#endif