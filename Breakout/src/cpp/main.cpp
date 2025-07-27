#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#define STB_IMAGE_IMPLEMENTATION // Preprocessor will modify the header file so it only contains relevant source code, effectively turning it into a .cpp.
#include "../headers/stb_image.h"
#include "../headers/game.h"
#include "../headers/resource_manager.h"
#include "../headers/texture.h"
#include "../headers/sprite_renderer.h"

using namespace std;
using namespace glm;

void processInput(GLFWwindow *window);

mat4 projection = ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);

int main() 
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Breakout", NULL, NULL);
    if(window == NULL)
    {
        cout << "Failed to create GLFW window!" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Makes the context of our window the main context on the current thread!

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // Set up GLAD. Uses your OS specific OpenGL functions!
    {
        cout << "Failed to initialize GLAD!" << endl;
        return -1;
    }

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}