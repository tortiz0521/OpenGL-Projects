#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "resource_manager.h"
#include "sprite_renderer.h"
#include <glm/glm.hpp>

class GameObject
{
public:
    glm::vec2 position, size, velo;
    glm::vec3 color;
    Texture2D sprite;
    bool alive = true, isSolid = false;
    float rotation;

    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D tex,
        glm::vec2 velo = glm::vec2(0.0f), glm::vec3 color = glm::vec3(1.0f),
        float rotation = 0.0f, bool solid = false);


    // Check if the object is alive.
    bool IsAlive() const;
    bool IsSolid() const;

    void Destroy(); // Set alive to false.

    void Draw(SpriteRenderer &renderer);
};

#endif