#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glm/glm.hpp>
#include "shader.h"
#include "texture.h"

using namespace glm;

class SpriteRenderer
{
public:
    SpriteRenderer(const Shader &shader);
    ~SpriteRenderer();

    // This Draw() function works a little bit differently from the one used in our complete model load. Instead of preparing the uniforms
    // then sending the shader currently in use, we set up the entire shader here!
    void Draw(const Texture2D &tex, const vec2 &position, const float rotate = 0.0f,
        const vec2 size = vec2(10.0f, 10.0f), const vec3 color = vec3(1.0f));

private:
    Shader s_;
    unsigned int VAO_;

    void initRenderer();
};

#endif // SPRITE_RENDERER_H