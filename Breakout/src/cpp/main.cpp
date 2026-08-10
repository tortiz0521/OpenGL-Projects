#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#define STB_IMAGE_IMPLEMENTATION // Preprocessor will modify the header file so it only contains relevant source code, effectively turning it into a .cpp.
#include "../headers/game.h"
#include "../headers/resource_manager.h"
#include "../headers/texture.h"
#include "../headers/sprite_renderer.h"

using namespace std;
using namespace glm;

bool setupWindow(const unsigned int w, const unsigned int h, GLFWwindow* &window);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

Game game = Game(WIDTH, HEIGHT);

int main() 
{
    GLFWwindow *window;
    if(!setupWindow(WIDTH, HEIGHT, window))
        return -1;

    // Setup the viewport!
    glViewport(0, 0, WIDTH, HEIGHT);

    game.init();

    // Blend function so that our paddle/ball are not just rectangles.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Establish callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    
    while(!glfwWindowShouldClose(window))
    {
        // Calculate deltatime!
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        // Process user input
        game.ProcessInput(deltaTime);

        // Update game state
        game.Update(deltaTime);

        // Render the frame!
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        game.Render();

        glfwSwapBuffers(window);
        //glfwPollEvents();
    }
}

bool setupWindow(const unsigned int w, const unsigned int h, GLFWwindow* &window)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    window = glfwCreateWindow(800, 600, "Breakout", NULL, NULL);
    if(window == NULL)
    {
        cout << "Failed to create GLFW window!" << endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window); // Makes the context of our window the main context on the current thread!

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // Set up GLAD. Uses your OS specific OpenGL functions!
    {
        cout << "Failed to initialize GLAD!" << endl;
        return false;
    }

    return true;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    // When the user presses the escape key, set the WindowShouldClose function to true.
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    // Get key inputs and determine if they are presses or releases.
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            game.Keys[key] = true;
        else if (action == GLFW_RELEASE)
            game.Keys[key] = false;  
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}