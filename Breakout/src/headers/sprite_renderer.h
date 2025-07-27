#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glm/glm.hpp>
#include <memory>
#include "shader.h"
#include "texture.h"

using namespace glm;

class SpriteRenderer
{
public:
    SpriteRenderer(Shader &shader);
    ~SpriteRenderer();

    // This Draw() function works a little bit differently from the one used in our complete model load. Instead of preparing the uniforms
    // then sending the shader currently in use, we set up the entire shader here!
    void Draw(Texture2D tex, vec3 &position, float rotate = 0.0f,
                vec2 size = vec2(10.0f, 10.0f), vec3 color = vec3(1.0f));

private:
    Shader s;
    std::shared_ptr<unsigned int> VAO, VBO;
    std::shared_ptr<float[]> verts;

    void initRenderData();
};

#endif // SPRITE_RENDERER_H