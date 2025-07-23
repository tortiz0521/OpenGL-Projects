#ifndef MESH_H
#define GAME_H

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

        // constructor and destructor
        Game(unsigned int w, unsigned int h);
        ~Game();

        // initialize game state (load shaders, texture, and levels)
        void init();

        // general game loop
        void ProcessInput(float dt); // Function takes in delta time.
        void Update(float dt); // Function takes in delta time.
        void Render();
};

#endif