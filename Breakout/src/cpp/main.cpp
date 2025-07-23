#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#define STB_IMAGE_IMPLEMENTATION // Preprocessor will modify the header file so it only contains relevant source code, effectively turning it into a .cpp.
#include "../headers/stb_image.h"
#include "../headers/game.h"

using namespace std;

void processInput(GLFWwindow *window);

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