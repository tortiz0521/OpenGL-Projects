#include "../headers/game_object.h"

GameObject::GameObject()
    : position_(0.0f, 0.0f), size_(1.0f, 1.0f), velo_(0.0f),
     color_(1.0f), rotation_(0.0f), sprite_() {}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D tex, glm::vec2 velo, glm::vec3 color, float rotation, bool solid) 
    : position_(pos), size_(size), sprite_(tex), rotation_(rotation), velo_(velo),
    color_(color), isSolid_(solid){}

void GameObject::Draw(SpriteRenderer &renderer)
{
    renderer.Draw(this->sprite_, this->position_, this->rotation_, this->size_, this->color_);
}

void GameObject::SetPosition(const glm::vec2 p)
{
    this->position_ = p;
}

void GameObject::SetSize(glm::vec2 s)
{
    this->size_ = s;
}

void GameObject::SetVelocity(glm::vec2 v)
{
    this->velo_ = v;
}

glm::vec2 GameObject::GetPosition() const
{
    return this->position_;
}

glm::vec2 GameObject::GetSize() const
{
    return this->size_;
}

glm::vec2 GameObject::GetVelocity() const
{
    return this->velo_;
}

glm::vec3 GameObject::GetColor() const
{
    return this->color_;
}

Texture2D GameObject::GetSpriteTex() const
{
    return this->sprite_;
}

float GameObject::GetRotation() const
{
    return this->rotation_;
}

bool GameObject::IsAlive() const
{
    return alive_;
}

bool GameObject::IsSolid() const
{
    return isSolid_;
}

void GameObject::Destroy()
{
    this->alive_ = false;
}