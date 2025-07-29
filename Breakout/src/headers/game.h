#ifndef MESH_H
#define GAME_H

// Include these libraries for key inputs into the game.
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "resource_manager.h"
#include "shader.h"
#include "texture.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include "game_level.h"
#include "ball.h"

#include <vector>


using namespace std;

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Game {
    public:
        //game state vars
        GameState State;
        bool Keys[1024];
        unsigned int Width, Height;

        // Game level vars
        vector<GameLevel> levels;
        unsigned int level;

        // constructor and destructor
        Game(unsigned int w, unsigned int h);
        ~Game();

        // initialize game state (load shaders, texture, and levels)
        void init();

        // general game loop
        void ProcessInput(float dt); // Function takes in delta time.
        void Update(float dt); // Function takes in delta time.
        void Render();
        void Collisions();
};

#endif