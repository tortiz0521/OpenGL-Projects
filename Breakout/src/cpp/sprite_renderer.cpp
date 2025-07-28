#include "../headers/sprite_renderer.h"

#include <glm/glm.hpp>

using namespace glm;

// Method definitions for the SpriteRenderer class.
SpriteRenderer::SpriteRenderer(const Shader &s)
{
    this->s_ = s;
    initRenderer();
}

void SpriteRenderer::initRenderer()
{
    unsigned int VBO; // VBO is not a class member variable because it is not needed for rendering.
    float vertices[] = {
        // pos      // texture
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    // Generate new objects.
    glGenVertexArrays(1, &this->VAO_);
    glGenBuffers(1, &VBO);

    // Bind the VBO variable as the current buffer pointer. Place in the buffer our sprite data.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    // Bind the current vertex array to the VAO variable. Enable an attribute pointer, then set what that pointer points to in memory.
    glBindVertexArray(this->VAO_);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    // Unbind the buffers.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SpriteRenderer::Draw(const Texture2D &tex, const vec2 &position, const float rotate,
    const vec2 size, const vec3 color)
{
    this->s_.Use();
    // Start with transformations (Remember order: scale -> rotate -> translate)
    mat4 model = mat4(1.0f);

    // Multiplying matrices occurs in right -> left order, so we start with a translation, then a rotation, and finally a scale.
    model = translate(model, vec3(position, 0.0f)); // Set the current position of our sprite in the model matrix.
    
    // Rotate about the center of the sprite.
    model = translate(model, vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, rotate, vec3(0.0f, 0.0f, 1.0f));
    model = translate(model, vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    // Scale the size of the sprite.
    model = scale(model, vec3(size, 1.0f));
    
    // Set the uniforms for our shaders.
    this->s_.SetMatrix4("model", model, true);
    this->s_.SetVector3f("spriteColor", color);

    // Set the texture to be used.
    glActiveTexture(GL_TEXTURE0);
    tex.Bind();

    // Bind the appropriate vertex array, and draw the sprite.
    glBindVertexArray(this->VAO_);
    glDrawArrays(GL_TRIANGLES, 0, 6); // Draw 2 triangles (6 vertices at a time).
    glBindVertexArray(0); // Unbind the VAO.
}