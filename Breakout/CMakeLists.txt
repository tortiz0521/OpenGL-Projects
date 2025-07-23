cmake_minimum_required(VERSION 3.10)

project(Breakout)

find_package(OpenGL REQUIRED)

find_package(glm CONFIG REQUIRED)

find_package(glad CONFIG REQUIRED)

find_package(glfw3 CONFIG REQUIRED)

find_package(assimp CONFIG REQUIRED)

add_executable(Breakout src/cpp/main.cpp)

target_link_libraries(Breakout PRIVATE
    OpenGL::GL
    glm::glm
    glad::glad
    glfw
    assimp::assimp
)