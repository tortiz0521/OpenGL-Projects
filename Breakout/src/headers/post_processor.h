#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <iostream>

#include "texture.h"
#include "shader.h"

class PostProcessor
{
public:
    bool m_chaos = false, m_shake = false, m_confuse = false;

    void init(Shader s, unsigned int W, unsigned int H);
    void BindPostprocess();
    void EndPostprocess();
    void PostprocessRender(float time);
    unsigned int GetTexture();

private:
    unsigned int m_fbo{}, m_msfbo{}; // framebuffer & multisampled fbo
    unsigned int m_rbo{};
    unsigned int m_vao{};

    // Texture info
    Texture2D m_texture{};
    Shader m_shader{};
    unsigned int m_width, m_height;

    bool m_initialized = false;
    void initRender();
};

#endif