#include "../headers/game.h"

#include <iostream>
#include <vector>

// Player Attributes
const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
const float PLAYER_VELOCITY(500.0f);
//Ball Attributes
const float BALL_RADIUS = 12.5f;
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);

SpriteRenderer *Renderer;
GameObject *Player;
BallObject *Ball;

Game::Game(unsigned int w, unsigned int h) : Width(w), Height(h) {
    State = GAME_ACTIVE;
}

Game::~Game()
{

}

void Game::init()
{
    // Load shaders and textures
    ResourceManager::LoadShader("src/shaders/sprite.vs", "src/shaders/sprite.fs", nullptr, "sprite");
    ResourceManager::LoadTexture("textures/awesomeface.png", "awesomeface");
    ResourceManager::LoadTexture("textures/block_solid.png", "block_solid");
    ResourceManager::LoadTexture("textures/block.png", "block");
    ResourceManager::LoadTexture("textures/paddle.png", "paddle");

    // Create levels and load them.
    GameLevel Standard, AFewSmallGaps, SpaceInvader, BounceGalore;
    Standard.Load("levels/standard.lvl", this->Width , this->Height / 2);
    AFewSmallGaps.Load("levels/a_few_small_gaps.lvl", this->Width , this->Height / 2);
    SpaceInvader.Load("levels/space_invader.lvl", this->Width , this->Height / 2);
    BounceGalore.Load("levels/bounce_galore.lvl", this->Width , this->Height / 2);

    // Place them into the levels member vector, set the current level.
    this->levels.push_back(Standard);
    this->levels.push_back(AFewSmallGaps);
    this->levels.push_back(SpaceInvader);
    this->levels.push_back(BounceGalore);
    this->level = 0;

    // Initialize the player.
    glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - (PLAYER_SIZE.x / 2.0f), this->Height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

    // Initialize the ball.
    Ball = new BallObject(glm::vec2(playerPos.x + (PLAYER_SIZE.x / 2.0f - BALL_RADIUS), playerPos.y + -(BALL_RADIUS * 2.0f)),
        BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("awesomeface"));

    // Set the orthographics projection matrix.
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f);
    
    // Configure shader-vertrex uniforms.
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
}

// Looking back on this function makes me cringe a little. Who care about the member variables being private??? It take so much more to do this.
void Game::ProcessInput(float dt) 
{
    if (this->State == GAME_ACTIVE) {
        float velo = PLAYER_VELOCITY * dt;

        if (this->Keys[GLFW_KEY_A]) {
            if (Player->GetPosition().x >= 0.0f) {
                Player->SetPosition(glm::vec2(Player->GetPosition().x - velo, Player->GetPosition().y));
                if (Ball->stuck) 
                    Ball->SetPosition(glm::vec2(Ball->GetPosition().x - velo, Ball->GetPosition().y));
            }
        }
        else if (this->Keys[GLFW_KEY_D]) {
            if (Player->GetPosition().x <= this->Width - Player->GetSize().x) {
                Player->SetPosition(glm::vec2(Player->GetPosition().x + velo, Player->GetPosition().y));
                if (Ball->stuck) 
                    Ball->SetPosition(glm::vec2(Ball->GetPosition().x + velo, Ball->GetPosition().y));
            }
            
        }
        else if (this->Keys[GLFW_KEY_SPACE]) 
            Ball->stuck = false;
    }
}

void Game::Update(float dt)
{
    Ball->Move(dt, this->Width);
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE) {
        // Draw the background first!
        Renderer->Draw(ResourceManager::LoadTexture("textures/background.jpg", "background"),
            glm::vec2(0.0f, 0.0f), 0.0f, glm::vec2(this->Width, this->Height));

        // Draw the player!
        Player->Draw(*Renderer);

        // Draw the Ball!
        Ball->Draw(*Renderer);

        // Draw the rest of the sprites!
        this->levels[this->level].Draw(*Renderer);
    }
}