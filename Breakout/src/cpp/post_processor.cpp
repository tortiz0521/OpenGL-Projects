#include "../headers/post_processor.h"

void PostProcessor::init(Shader s, unsigned int W, unsigned int H)
{
    if (m_initialized) return;

    m_width = W;
    m_height = H;
    m_shader = s;

    initRender();

    // Generate framebuffer
    glGenFramebuffers(1, &m_fbo);
    glGenFramebuffers(1, &m_msfbo);

    // Generate texture attachment
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    m_texture.Generate(W, H, NULL);
    // Attach teture to the framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture.ID, 0);
    // Check Framebuffer status
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::POSTPROCESSOR::Failed to initialize FBO." << std::endl;
    }

    // Generate renderbuffer object to handle the depth and stencil tests.
    glBindFramebuffer(GL_FRAMEBUFFER, m_msfbo);
    glGenRenderbuffers(1, &m_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, W, H);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    // Attach renderbuffer to the framebuffer

    // Check Framebuffer status
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::POSTPROCESSOR::Failed to initialize MSFBO." << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_shader.SetInteger("scene", 0, true);
    float offset = 1.0f / 300.0f;
    float offsets[9][2] = {
        { -offset,  offset  },  // top-left
        {  0.0f,    offset  },  // top-center
        {  offset,  offset  },  // top-right
        { -offset,  0.0f    },  // center-left
        {  0.0f,    0.0f    },  // center-center
        {  offset,  0.0f    },  // center - right
        { -offset, -offset  },  // bottom-left
        {  0.0f,   -offset  },  // bottom-center
        {  offset, -offset  }   // bottom-right    
    };
    glUniform2fv(glGetUniformLocation(m_shader.ID, "offsets"), 9, (float*)offsets);

    int edge_kernel[9] = {
        -1, -1, -1,
        -1,  8, -1,
        -1, -1, -1
    };
    glUniform1iv(glGetUniformLocation(m_shader.ID, "edgeKernel"), 9, edge_kernel);

    float blur_kernel[9] = {
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
        2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
    };
    glUniform1fv(glGetUniformLocation(m_shader.ID, "blurKernel"), 9, blur_kernel);   

    m_initialized = true;
}

void PostProcessor::BindPostprocess()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PostProcessor::EndPostprocess()
{
    glBindFramebuffer(GL_READ_BUFFER, m_msfbo);
    glBindBuffer(GL_DRAW_BUFFER, m_fbo);
    glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::PostprocessRender(float time)
{
    m_shader.Use();
    m_shader.SetFloat("time", time);
    m_shader.SetInteger("confuse", m_confuse);
    m_shader.SetInteger("shake", m_shake);
    m_shader.SetInteger("chaos", m_chaos);

    // Bind generated texture from postprocess FBO to default FBO.
    glActiveTexture(GL_TEXTURE0);
    m_texture.Bind();
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

unsigned int PostProcessor::GetTexture()
{
    return m_texture.ID;
}

void PostProcessor::initRender()
{
    unsigned int vbo{};
    float vertices[] = {
        // pos        // tex
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,

        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f
    };

    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &m_vao);

    // VBO data
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // VAO data
    glBindVertexArray(m_vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}