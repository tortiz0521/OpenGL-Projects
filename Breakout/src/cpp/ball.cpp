#include "../headers/ball.h"

#include <iostream>

BallObject::BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite)
    : GameObject(pos, glm::vec2(radius * 2.0f), sprite, velocity), radius(radius) {}

BallObject::BallObject() : GameObject(), radius(12.5f) {}

glm::vec2 BallObject::Move(float dt, unsigned int window_width)
{
    if (!this->stuck) {
        // Set the current velocity based on delta time.
        glm::vec2 tempP(this->GetPosition() + (this->GetVelocity() * dt));
        this->SetPosition(tempP);

        // Check positions for wall colisions
        if (this->GetPosition().x >= window_width - this->GetSize().x ||
            this->GetPosition().x <= 0.0f)
            this->SetVelocity(glm::vec2(-this->GetVelocity().x, this->GetVelocity().y));
        else if (this->GetPosition().y <= 0.0f)
            this->SetVelocity(glm::vec2(this->GetVelocity().x, -this->GetVelocity().y));
    }

    return this->GetPosition();
}

void BallObject::Reset(glm::vec2 position, glm::vec2 velocity)
{
    this->SetPosition(position);
    this->SetVelocity(velocity);
    this->stuck = true;
}