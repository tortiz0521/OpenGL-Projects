#include "../headers/game_object.h"

GameObject::GameObject()
    : position(0.0f, 0.0f), size(1.0f, 1.0f), velo(0.0f),
     color(1.0f), rotation(0.0f), sprite() {}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D tex, glm::vec2 velo, glm::vec3 color, float rotation, bool solid) 
    : position(pos), size(size), sprite(tex), rotation(rotation), velo(velo),
    color(color), isSolid(solid){}

void GameObject::Draw(SpriteRenderer &renderer)
{
    renderer.Draw(this->sprite, this->position, this->rotation, this->size, this->color);
}

bool GameObject::IsAlive() const
{
    return alive;
}

bool GameObject::IsSolid() const
{
    return isSolid;
}

void GameObject::Destroy()
{
    this->alive = false;
}