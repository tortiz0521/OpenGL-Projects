#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "shader.h"
#include "camera.h"
#include "model.h"

//#define STB_IMAGE_IMPLEMENTATION // Preprocessor will modify the header file so it only contains relevant source code, effectivvely turning it into a .cpp.
//#include "stb_image.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void processInput(GLFWwindow* window, float &mix);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

const unsigned int WIDTH = 800, HEIGHT = 600;
float lastX = (float)WIDTH / 2.0f, lastY = (float)HEIGHT / 2.0f;

vec3 cameraPos   = vec3(0.0f, 0.0f,  3.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp    = vec3(0.0f, 1.0f,  0.0f);

// Set 'yaw' to -90 degrees to ensure that we are initially pointing towards the negative z-axis.
Camera cam(cameraPos, cameraFront, cameraUp, 2.5f, 0.0f, -90.0f, 0.1f, 45.0f);

float deltaTime= 0.0f, lastFrame = 0.0f;
bool firstMouse = true;

// Light sources!
vec3 lightPos(1.2f, 1.0f, 2.0f);

int main() {
    // Initialize and set up the glfwWindow!
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "GameEngine", NULL, NULL); // Create the window.
    if(window == NULL) 
    {
        cout << "Failed to create GLFW window!" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Makes the context of our window the main context on the current thread!

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // Set up GLAD. Uses your OS specific OpenGL functions!
    {
        cout << "Failed to intialze GLAD!" << endl;
        return -1;
    }

    Shader shaders[2] = {
        Shader("vertex.vs", "fragment1.fs"),
        Shader("vertex.vs", "fragment1.fs")
    };

    Shader ModelLoad = Shader("vertex.vs", "simpleF.fs");
    Shader ScaledUpModel = Shader("vertex.vs", "border.fs");

    // Loading a texture is similar to loading in vertext information to a VAO.
    //stbi_set_flip_vertically_on_load(true);
    //unsigned int texture1 = loadTexture("textures/container2.png"), texture2 = loadTexture("textures/container2_specular.png"), texture3 = loadTexture("textures/matrix.jpg");
    Model backpack("backpack/backpack.obj");
    cout << "here" << endl;

    // Bind the texture to set data for.
        //glBindTexture(GL_TEXTURE_2D, texture1);
    // Arg1: texture target; Arg2: option type per axis; Arg3: Texture wrrapping mode.
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // For a GL_CLAMP_TO_BORDER arg3, specify a border color!
        //float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
        //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor); 

    /*  Texture filtering: Use nearest to select the nearest texel from a specified texture coordinate on a texture.
        Use linear to produce an output that interpolates the specific pixel based on the texels around the specified coordinate.
        Use these two functions below as a way to set the options being used: 
    */
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /*  Mipmaps: You alredy know what they do, but its important to know the options you have with them.
        Similarly to a standard texture filtering, you can also use nearest/linear for switching through
        mipmaps. The options are shown below:
            GL_NEAREST_MIPMAP_NEAREST: takes the nearest mipmap to match the pixel size and uses nearest neighbor interpolation for texture sampling.
            GL_LINEAR_MIPMAP_NEAREST: takes the nearest mipmap level and samples that level using linear interpolation.
            GL_NEAREST_MIPMAP_LINEAR: linearly interpolates between the two mipmaps that most closely match the size of a pixel and samples the interpolated level via nearest neighbor interpolation.
            GL_LINEAR_MIPMAP_LINEAR: linearly interpolates between the two closest mipmaps and samples the interpolated level via linear interpolation.
        Below are methods that can be set to do the appropriate filtering of mipmaps:
    */
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // You cannot give a mipmap filtering option to magnification, it will return the error code: GL_INVALID_ENUM
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /*  Now we generate a texture! Its a large function, so here is everything that goes into it:
            The first argument specifies the texture target; setting this to GL_TEXTURE_2D means this operation will generate a texture on the currently bound texture object at the same target (so any textures bound to targets GL_TEXTURE_1D or GL_TEXTURE_3D will not be affected).
            The second argument specifies the mipmap level for which we want to create a texture for if you want to set each mipmap level manually, but we'll leave it at the base level which is 0.
            The third argument tells OpenGL in what kind of format we want to store the texture. Our image has only RGB values so we'll store the texture with RGB values as well.
            The 4th and 5th argument sets the width and height of the resulting texture. We stored those earlier when loading the image so we'll use the corresponding variables.
            The next argument should always be 0 (some legacy stuff).
            The 7th and 8th argument specify the format and datatype of the source image. We loaded the image with RGB values and stored them as chars (bytes) so we'll pass in the corresponding values.
            The last argument is the actual image data.
    */

    
    float mix = 0.2;

    // Create viewport and establish callback function for size changes.
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Set the mode shapes are drawn in!
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Tell OpenGL to use the z-buffer!
    glEnable(GL_DEPTH_TEST);

    // Tell OpenGL to use the stencil buffer/stencil test.
    glEnable(GL_STENCIL_TEST);

    // Tell the window to hide the cursor and 'capture' it to the screen.
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    cout << "now here" << endl;
    // Keep window alive and update the program. (Render Loop)
    while(!glfwWindowShouldClose(window))
    {
        // Update deltaTime!
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //input
        processInput(window, mix);
        glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Specify which actions should be taken whenever any of the stencil tests succeed of fail.
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments will pass the test this first time!
        glStencilMask(0xFF); // enable writing to the stencil buffer.

        //state changes
        const float radius = 10.0f;

        //render
        mat4 model = mat4(1.0f), projection;
        projection = perspective(radians(cam.getFOV()), 800.0f / 600.0f, 0.1f, 100.0f);
        model = rotate(model, radians((float)glfwGetTime() * 15.0f), vec3(0.0f, 1.0f, 0.0f));
        ModelLoad.use();
        ModelLoad.setMatrix4fv("model", model);
        ModelLoad.setMatrix4fv("view", cam.getView());
        ModelLoad.setVec3("viewPos", cam.getPos());
        ModelLoad.setMatrix4fv("projection", projection);
        ModelLoad.setBool("scale", false);

        // Directional Light material setup.
        ModelLoad.setVec3("dirLight.direction", vec3(-0.2f, 0.0f, -0.3f));
        ModelLoad.setVec3("dirLight.ambient", vec3(0.1f));
        ModelLoad.setVec3("dirLight.diffuse", vec3(0.8f));
        ModelLoad.setVec3("dirLight.specular", vec3(0.5f));
        ModelLoad.setFloat("material.shininess", 64.0f);

        backpack.Draw(ModelLoad);

        /* 
            Now that the stencil buffer is updated with 1s where the containers were drawn we're going to draw the upscaled containers,
            but this time with the appropriate test function and disabling writes to the stencil buffer: 
        */
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00); // disable writing to the stencil buffer.

        ScaledUpModel.use();
        ScaledUpModel.setMatrix4fv("model", model);
        ScaledUpModel.setMatrix4fv("view", cam.getView());
        ScaledUpModel.setVec3("viewPos", cam.getPos());
        ScaledUpModel.setMatrix4fv("projection", projection);
        ScaledUpModel.setBool("scale", true);
        backpack.Draw(ScaledUpModel);

        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glEnable(GL_DEPTH_TEST);

        // Note: Bind the vertex array BEFORE setting NEW uniforms. We don't want to use outdated data when we bind! If a uniform value never changes, this doesn't matter.

        // check and call event and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, float &mix) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
        mix = std::min((mix + 0.01), 1.0);
    if(glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
        mix = std::max((mix - 0.01), 0.0);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.CamPosTranslate(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.CamPosTranslate(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.CamPosTranslate(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.CamPosTranslate(RIGHT, deltaTime);
    
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
        return;
    }

    float xOffset = xpos - lastX, yOffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    //const float sensitivity = 0.1;
    cam.MouseProcess(xOffset, yOffset, false);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    cam.ScrollProcess(yoffset);
}