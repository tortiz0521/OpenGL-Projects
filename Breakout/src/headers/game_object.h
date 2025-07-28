#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "resource_manager.h"
#include "sprite_renderer.h"
#include <glm/glm.hpp>

class GameObject
{
public:
    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D tex,
        glm::vec2 velo = glm::vec2(0.0f), glm::vec3 color = glm::vec3(1.0f),
        float rotation = 0.0f, bool solid = false);

    // Setters
    void SetPosition(const glm::vec2 p);
    void SetSize(glm::vec2 s);
    void SetVelocity(glm::vec2 v);

    // Getters
    glm::vec2 GetPosition() const;
    glm::vec2 GetSize() const;
    glm::vec2 GetVelocity() const;
    glm::vec3 GetColor() const;
    Texture2D GetSpriteTex() const;
    float GetRotation() const;

    // Check if the object is alive.
    bool IsAlive() const;
    bool IsSolid() const;

    void Destroy(); // Set alive to false.

    void Draw(SpriteRenderer &renderer);

private:
    glm::vec2 position_, size_, velo_;
    glm::vec3 color_;
    Texture2D sprite_;
    bool alive_ = true, isSolid_ = false;
    float rotation_;
};

#endif