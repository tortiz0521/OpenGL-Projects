#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H

#include "game_object.h"
#include "sprite_renderer.h"

#include <vector>

class GameLevel
{
public:
    GameLevel() {};

    // Load a level from a file!
    void Load(const char *file, unsigned int levelWidth, unsigned int levelHeight);

    // Render the level.
    void Draw(SpriteRenderer &renderer);

    // Check if the level has been completed.
    bool IsComplete();

private:
    std::vector<GameObject> Bricks;

    // Initialize the level from our tile data.
    void init(std::vector<std::vector<unsigned int>> tileData, unsigned int lw, unsigned int lh);
};

#endif