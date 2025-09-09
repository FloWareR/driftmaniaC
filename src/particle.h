#ifndef PARTICLE_H
#define PARTICLE_H

#include <raylib.h>

//====================================================================================
// Defines and Macros
//====================================================================================
#define MAX_PARTICLES 1024

//====================================================================================
// Structs and Typedefs
//====================================================================================
typedef struct Particle{
    Vector2 position;
    Color color;
    float rotation;
    float size;
    float lifetime;
    float lifeDuration;
    bool isActive;
} Particle;

typedef struct ParticleSystem{
    Particle particles[MAX_PARTICLES];
} ParticleSystem;

//====================================================================================
// Function Prototypes
//====================================================================================
void InitParticleSystem(ParticleSystem *system);
void UpdateParticleSystem(ParticleSystem *system, float dt);
void DrawParticleSystem(const ParticleSystem *system);
void EmitSkidParticle(ParticleSystem *system, Vector2 position, float rotation, float size);

#endif // PARTICLE_H