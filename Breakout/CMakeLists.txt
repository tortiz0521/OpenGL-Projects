cmake_minimum_required(VERSION 3.10)

project(Breakout)

find_package(OpenGL REQUIRED)

find_package(glm CONFIG REQUIRED)

find_package(glad CONFIG REQUIRED)

find_package(glfw3 CONFIG REQUIRED)

find_package(assimp CONFIG REQUIRED)

add_executable(Breakout)

target_sources(Breakout PRIVATE
    src/cpp/main.cpp
    src/cpp/game.cpp
    src/cpp/resource_manager.cpp
    src/cpp/shader.cpp
    src/cpp/sprite_renderer.cpp
    src/cpp/texture.cpp
    src/cpp/game_object.cpp
    src/cpp/game_level.cpp
    src/cpp/ball.cpp
)

target_link_libraries(Breakout PRIVATE
    OpenGL::GL
    glm::glm
    glad::glad
    glfw
    assimp::assimp
)