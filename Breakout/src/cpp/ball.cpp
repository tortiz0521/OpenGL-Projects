#include "../headers/ball.h"

#include <iostream>

BallObject::BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite)
    : GameObject(pos, glm::vec2(radius * 2.0f), sprite, velocity), radius(radius) {}

BallObject::BallObject() : GameObject(), radius(12.5f) {}

glm::vec2 BallObject::Move(float dt, unsigned int window_width)
{
    if (!this->stuck) {
        // Set the current velocity based on delta time.
        this->position += this->velo * dt;

        // Check positions for wall colisions
        if (this->position.x >= window_width - this->size.x ||
            this->position.x <= 0.0f)
            this->velo.x = -this->velo.x;
        else if (this->position.y <= 0.0f)
            this->velo.y = -this->velo.y;
    }

    return this->position;
}

void BallObject::Reset(glm::vec2 position, glm::vec2 velocity)
{
    this->position = position;
    this->velo = velocity;
    this->stuck = true;
}