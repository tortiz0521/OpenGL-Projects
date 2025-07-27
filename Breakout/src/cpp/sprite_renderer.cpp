#include "../headers/sprite_renderer.h"

#include <glm/glm.hpp>

using namespace glm;

unsigned int VAO, VBO;
float vertices[] = {
    // pos      // texture
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f
};

void setupSpriteObjects(unsigned int &VAO, unsigned int &VBO)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void SpriteRenderer::initRenderData() // Configure the VAO/VBO
{
    
}