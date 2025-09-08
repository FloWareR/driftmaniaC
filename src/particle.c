#include "particle.h"
#include <raymath.h>

/**
 * @brief Initializes the particle system by deactivating all particles.
 */
void InitParticleSystem(ParticleSystem *system){
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        system->particles[i].isActive = false;
    }
}

/**
 * @brief Updates all active particles each frame.
 */
void UpdateParticleSystem(ParticleSystem *system, float dt){
    for (int i =0; i < MAX_PARTICLES; i++){
        if (system->particles[i].isActive){
            system->particles[i].lifetime += dt;
            // Deactivate particle if its lifetime exceeds its duration
            if (system->particles[i].lifetime >= system->particles[i].lifeDuration){
                system->particles[i].isActive = false;
            }
        }
    }
}

/**
 * @brief Renders all active particles.
 */
void DrawParticleSystem(const ParticleSystem *system){
    for (int i = 0; i < MAX_PARTICLES; i++){
        if(system->particles[i].isActive){
            Particle p = system->particles[i];

            // Fade the particle out over it's lifetime
            float life_ratio = (p.lifetime / p.lifeDuration);
            Color c = Fade(p.color, 1.0f - life_ratio);
            
            Rectangle dest = {p.position.x, p.position.y, p.size, p.size / 2.0f};
            Vector2 origin = {p.size/2.0f, p.size/4.0f};
            DrawRectanglePro(dest, origin, p.rotation, c);
        }
    }
}

void EmitSkidParticle(ParticleSystem *ps, Vector2 position, float rotation, float size) {
       for (int i = 0; i < MAX_PARTICLES; i++) {
        
        // Find the first available inactive particle
        if (!ps->particles[i].isActive) {
            ps->particles[i] = (Particle){
                .position = position,
                .color = (Color){40, 40, 40, 150},
                .rotation = rotation,
                .size = size,
                .lifetime = 0.0f,
                .lifeDuration = 3.0f, 
                .isActive = true
            };
            return; 
        }
    }
}
