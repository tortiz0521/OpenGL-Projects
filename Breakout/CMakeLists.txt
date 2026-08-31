cmake_minimum_required(VERSION 3.10)

project(Breakout)

add_executable(Breakout)
set_target_properties(Breakout PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin/Debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin/Release"
)

find_package(OpenGL REQUIRED)

find_package(glm CONFIG REQUIRED)

find_package(glad CONFIG REQUIRED)

find_package(glfw3 CONFIG REQUIRED)

find_package(assimp CONFIG REQUIRED)

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
    src/cpp/particle.cpp
    src/cpp/post_processor.cpp
)

target_link_libraries(Breakout PRIVATE
    OpenGL::GL
    glm::glm
    glad::glad
    glfw
    assimp::assimp
)