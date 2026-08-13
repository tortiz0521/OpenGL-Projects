#ifndef PARTICLE_H
#define PARTICLE_H
    
#include <glm/glm.hpp>
#include <vector>

#include "shader.h"
#include "texture.h"
#include "game_object.h"


struct Particle {
    glm::vec2 Position{}, Velo{};
    glm::vec4 Color{};
    float Life{};
    
    Particle() : Position(0.0f), Velo(0.0f), Color(1.0f), Life(0.0f) { }
};


class ParticleGenerator {
public:
    ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount);
    void Update(float dt, GameObject &object, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f), bool createNew = true);
    void Draw();

private:
    unsigned int m_amount{}, m_lastParticleUsed = 0;
    std::vector<Particle> m_particles{};
    Shader m_shader{};
    Texture2D m_texture{};

    unsigned int m_VAO{};

    void m_RespawnParticle(Particle &particle, GameObject &object, glm::vec2 offset = glm::vec2(0.0f));
    unsigned int m_FirstUnusedParticle();
    void m_init();
};

#endif