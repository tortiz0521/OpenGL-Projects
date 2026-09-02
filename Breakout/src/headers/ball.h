#ifndef BALL_H
#define BALL_H

#include "game_object.h"

class BallObject : public GameObject
{
public:
    // The ball state vars
    float radius;
    bool stuck = true;
    bool isSolid = true;
    bool isSticky = false;

    BallObject();
    BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);

    glm::vec2 Move(float dt, unsigned int window_width);
    void Reset(glm::vec2 position, glm::vec2 velocity);
};

#endif