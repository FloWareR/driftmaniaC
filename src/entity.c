#include "entity.h"
#include <raymath.h>

#define COLLECTABLE_COLOR GOLD
#define OBSTACLE_COLOR GRAY

void InitEntityManager(EntityManager *em)
{
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        em->entities[i].isActive = false;
    }
}

void DrawEntities(const EntityManager *em, bool isDebug)
{
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        if (em->entities[i].isActive)
        {
            Entity entity = em->entities[i];
            // Draw the main entity (as a colored box)
            DrawRectangleRec(entity.bounds, entity.color);

            // --- ADD THIS BLOCK TO DRAW ENTITY HITBOX ---
            if (isDebug)
            {
                // Draw a red outline around the entity's bounds
                DrawRectangleLinesEx(entity.bounds, 2.0f, RED);
            }
            // --- END OF NEW BLOCK ---
        }
    }
}

void SpawnEntity(EntityManager *em, EntityType type, Vector2 position)
{
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        if (!em->entities[i].isActive)
        {
            em->entities[i] = (Entity){
                .position = position,
                .isActive = true,
                .type = type};

            float size = (type == OBSTACLE_ROCK) ? 50.0f : 25.0f;
            em->entities[i].bounds = (Rectangle){position.x - size / 2, position.y - size / 2, size, size};
            em->entities[i].color = (type == OBSTACLE_ROCK) ? OBSTACLE_COLOR : COLLECTABLE_COLOR;

            return; // Exit after spawning one
        }
    }
}

void CheckPlayerEntityCollisions(Player *player, EntityManager *em)
{

    Rectangle playerBounds = {
        player->position.x - player->origin.x,
        player->position.y - player->origin.y,
        player->width,
        player->height};

    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        if (em->entities[i].isActive)
        {
            Entity *entity = &em->entities[i];

            if (CheckCollisionRecs(playerBounds, entity->bounds))
            {
                // Handle collision based on the entity type.
                switch (entity->type)
                {
                case COLLECTABLE_SCORE:
                    player->score += 100;
                    entity->isActive = false;
                    // TODO: Play a collection sound effect
                    break;

                case OBSTACLE_ROCK:
                {
                    // Get the rectangle that represents the overlap area.
                    Rectangle overlap = GetCollisionRec(playerBounds, entity->bounds);

                    //    We push the player out along the shortest axis of overlap.
                    if (overlap.width < overlap.height)
                    {
                        // Horizontal collision: Push the player left or right.
                        if (playerBounds.x + playerBounds.width / 2 < entity->bounds.x + entity->bounds.width / 2)
                        {
                            // Player is on the left side of the obstacle, push left.
                            player->position.x -= overlap.width;
                        }
                        else
                        {
                            // Player is on the right side of the obstacle, push right.
                            player->position.x += overlap.width;
                        }
                    }
                    else
                    {
                        // Vertical collision: Push the player up or down.
                        if (playerBounds.y + playerBounds.height / 2 < entity->bounds.y + entity->bounds.height / 2)
                        {
                            // Player is above the obstacle, push up.
                            player->position.y -= overlap.height;
                        }
                        else
                        {
                            // Player is below the obstacle, push down.
                            player->position.y += overlap.height;
                        }
                    }

                    // Stop the player's movement on impact.
                    player->velocity = (Vector2){0, 0};
                    player->forwardSpeed = 0;

                    break; // Don't forget the break!
                }
                }
            }
        }
    }
}