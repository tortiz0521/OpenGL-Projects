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
    float rotation;

    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D tex,
        glm::vec2 velo = glm::vec2(0.0f), glm::vec3 color = glm::vec3(1.0f),
        float rotation = 0.0f, bool solid = false
    );


    // Check if the object is alive.
    bool IsAlive() const;
    bool IsSolid() const;

    void Destroy(); // Set alive to false.

    void Draw(SpriteRenderer &renderer);

private:
    bool alive = true, isSolid = false;
};


class PowerUp : public GameObject
{
public:
    std::string m_type{};
    float m_duration{};
    bool m_activated{};

    PowerUp(std::string type, glm::vec2 pos, Texture2D tex, float duration, glm::vec3 color = glm::vec3(0.0f)) 
        : GameObject(pos, glm::vec2(60.0f, 20.0f), tex, glm::vec2(0.0f, 75.0f), color),
        m_type(type), m_duration(duration), m_activated(false) 
    { };
};

#endif