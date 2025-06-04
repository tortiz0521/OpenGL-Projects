cmake_minimum_required(VERSION 3.10)

project(GameEngine)

find_package(OpenGL REQUIRED)

find_package(glm CONFIG REQUIRED)

find_package(glad CONFIG REQUIRED)

find_package(glfw3 CONFIG REQUIRED)

find_package(assimp CONFIG REQUIRED)

add_executable(GameEngine advancedGL.cpp)

target_link_libraries(GameEngine PRIVATE
    OpenGL::GL
    glm::glm
    glad::glad
    glfw
    assimp::assimp
)