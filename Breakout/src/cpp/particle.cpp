#include "../headers/particle.h"
#include <iostream>

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount)
    : m_shader(shader), m_texture(texture), m_amount(amount)
{
    m_init();
}

void ParticleGenerator::Update(float dt, GameObject &object, unsigned int newParticles, glm::vec2 offset, bool createNew)
{
    if (createNew) {
        for (unsigned int i = 0; i < newParticles; ++i) {
            int index = m_FirstUnusedParticle();
            m_RespawnParticle(m_particles[index], object, offset);
        }
    }

    for (unsigned int i = 0; i < m_amount; ++i) {
        Particle &p = m_particles[i];
        p.Life -= dt;
        if (p.Life > 0.0f) {
            p.Position -= p.Velo * dt;
            p.Color.a -= dt * 2.5f;
        }
    }
}

void ParticleGenerator::Draw()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    m_shader.Use();
    unsigned int counter = 0;
    for (Particle &particle : m_particles) {
        if (particle.Life > 0.0f) {
            m_shader.SetVector2f("offset", particle.Position);
            m_shader.SetVector4f("color", particle.Color);
            m_texture.Bind();
            glBindVertexArray(this->m_VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
            counter++;
        }
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


// PRIVATE FUNCTIONS

void ParticleGenerator::m_init()
{
    unsigned int VBO;
    float particle_quad[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &this->m_VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
    glBindVertexArray(0);

    for (unsigned int i = 0; i < this->m_amount; i++)
        this->m_particles.push_back(Particle());
}

void ParticleGenerator::m_RespawnParticle(Particle &particle, GameObject &object, glm::vec2 offset)
{
    float random = ((rand() % 100) - 50) / 10.0f;
    float rColor = 0.5f + ((rand() % 100) / 100.0f);
    particle.Position = object.position + random + offset;
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.Life = 1.0f;
    particle.Velo = object.velo * 0.1f;
}

unsigned int ParticleGenerator::m_FirstUnusedParticle()
{
        // first search from last used particle, this will usually return almost instantly
    for (unsigned int i = m_lastParticleUsed; i < m_amount; ++i){
        if (this->m_particles[i].Life <= 0.0f){
            m_lastParticleUsed = i;
            return i;
        }
    }
    // otherwise, do a linear search
    for (unsigned int i = 0; i < m_lastParticleUsed; ++i){
        if (this->m_particles[i].Life <= 0.0f){
            m_lastParticleUsed = i;
            return i;
        }
    }
    // all particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
    m_lastParticleUsed = 0;
    return 0;

    /*unsigned int first_unused = std::find_if(m_particles.begin() + m_lastParticleUsed, m_particles.end(), 
        [](Particle &p) { return p.Life <= 0.0f; }
    ) - m_particles.begin();

    if (first_unused == m_particles.size()) {
        first_unused = std::find_if(m_particles.begin(), m_particles.begin() + m_lastParticleUsed, 
            [](Particle &p) { return p.Life <= 0.0f; }
        ) - m_particles.begin();
    }

    if (first_unused == m_particles.size())
        first_unused = 0;

    m_lastParticleUsed = first_unused;
    return first_unused;*/
}