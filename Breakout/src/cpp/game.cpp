#include "../headers/game.h"
#include "../headers/particle.h"
#include "../headers/post_processor.h"

#include <iostream>
#include <vector>
#include <algorithm>

// Player Attributes
const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
const float PLAYER_VELOCITY(500.0f);
//Ball Attributes
const float BALL_RADIUS = 12.5f;
const glm::vec2 INITIAL_BALL_VELOCITY(75.0f, -150.0f);

float shakeTime = 0.0f;


SpriteRenderer *Renderer;
GameObject *Player;
BallObject *Ball;
ParticleGenerator *Particles;
PostProcessor *Post;


/*
    This function acts as a way to check for AABB - AABB collision between two game objects.
    It checks if the ball objects furthest axes are greater than the bricks position axis and
    vice-versa. If both are true, then we know that there is some collision across the axis.
*/
bool CheckBoxCollision(GameObject &brick, GameObject &ball)
{
    bool xCollision = ball.position.x + ball.size.x >= brick.position.x &&
        ball.position.x <= brick.position.x + brick.size.x;
    
    bool yCollision = ball.position.y + ball.size.y >= brick.position.y &&
        ball.position.y <= brick.position.y + brick.size.y;
    
    return xCollision && yCollision;
}

void CollisionAdjust(BallObject &ball, const glm::vec2 &clamp, const glm::vec2 &D)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),	// up
        glm::vec2(1.0f, 0.0f),	// right
        glm::vec2(0.0f, -1.0f),	// down
        glm::vec2(-1.0f, 0.0f)	// left
    };
    float max = 0.0f;
    unsigned int best_match = -1;
    glm::vec2 nD = glm::length(D) == 0.0f ? glm::vec2(0.0f, 1.0f) : glm::normalize(D);
    for (unsigned int i = 0; i < 4; i++)
    {
        float dot_product = glm::dot(nD, compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }

    if (glm::length(ball.velo) == 0.0f)
        return;

    if (best_match == -1) {
        std::cout << "ISSUE HERE" << std::endl;
        return;
    }

    if (glm::dot(glm::normalize(ball.velo), glm::normalize(compass[best_match])) <= 0.0f)
        return;

    glm::vec2 r(0.f), R(0.0f);
    if (glm::length(D) != 0.0f) {
        r = glm::normalize(D) * ball.radius;
        R = r - D;
    }

    // Move the ball to fit.
    ball.position -= R;

    // Adjust the velocity of the ball.
    if (best_match == 0 || best_match == 2)
        ball.velo.y = -ball.velo.y;
    else {
        ball.velo.x = -ball.velo.x;
    }
}

/*
    This function is a bit different from the AABB-AABB collision above. Here we calculate the distance
    from our circle's center to the AABB's center, then we clamp that value to some point on the AABB.
    Lastly, determine if the distance from the circle's center to the clamped point, P, is less than the
    radius of the circle.
*/
bool CheckCircleCollision(GameObject &brick, BallObject &ball)
{
    // Find our half extents.
    glm::vec2 AABB_center = glm::vec2((brick.position.x + (brick.size.x / 2.0f)), (brick.position.y + (brick.size.y / 2.0f)));
    // std::cout << AABB_center.x << ", " << AABB_center.y << '\n';
    glm::vec2 circle_center = glm::vec2((ball.position.x + (ball.size.x / 2.0f)), (ball.position.y + (ball.size.y / 2.0f)));

    // Calculate the difference vector from the AABB object to our circle.
    glm::vec2 D = glm::vec2(circle_center.x - AABB_center.x, circle_center.y - AABB_center.y);
    
    // Find point P on the AABB: clamp the difference vector to be within the AABB, then add AABB_center to get the actual vector to P.
    glm::vec2 clamp = glm::clamp(D, -brick.size / 2.0f, brick.size / 2.0f);
    glm::vec2 P = AABB_center + clamp;

    // Update D to D_prime by subtracting P by the ball's center.
    D = P - circle_center;

    // return whether or not the ball's radius is >= the length to P from the center.
    bool collides = glm::length(D) <= ball.radius;
    if (collides)  
        CollisionAdjust(ball, clamp, D);
    return collides;
}

Game::Game(unsigned int w, unsigned int h) : Width(w), Height(h) {
    State = GAME_ACTIVE;
}

Game::~Game()
{

}

void Game::init()
{
    // Load shaders and textures
    ResourceManager::LoadShader("../../src/shaders/sprite.vs", "../../src/shaders/sprite.fs", nullptr, "sprite");
    ResourceManager::LoadShader("../../src/shaders/ball_particle.vs", "../../src/shaders/ball_particle.fs", nullptr, "particle");
    ResourceManager::LoadShader("../../src/shaders/post.vs", "../../src/shaders/post.fs", nullptr, "post");

    ResourceManager::LoadTexture("../../textures/awesomeface.png", "awesomeface");
    ResourceManager::LoadTexture("../../textures/block_solid.png", "block_solid");
    ResourceManager::LoadTexture("../../textures/block.png", "block");
    ResourceManager::LoadTexture("../../textures/paddle.png", "paddle");
    ResourceManager::LoadTexture("../../textures/particle.png", "particle");
    ResourceManager::LoadTexture("../../textures/background.jpg", "background");

    // Create levels and load them.
    GameLevel Standard, AFewSmallGaps, SpaceInvader, BounceGalore;
    Standard.Load("../../levels/standard.lvl", this->Width , this->Height / 2);
    AFewSmallGaps.Load("../../levels/a_few_small_gaps.lvl", this->Width , this->Height / 2);
    SpaceInvader.Load("../../levels/space_invader.lvl", this->Width , this->Height / 2);
    BounceGalore.Load("../../levels/bounce_galore.lvl", this->Width , this->Height / 2);

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
    ResourceManager::GetShader("particle").Use().SetMatrix4("projection", projection);

    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

    Particles = new ParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 5000);

    Post = new PostProcessor();
    Post->init(ResourceManager::GetShader("post"), 800, 600);
}

// Looking back on this function makes me cringe a little. Who care about the member variables being private??? It take so much more to do this.
void Game::ProcessInput(float dt) 
{
    if (this->State == GAME_ACTIVE) {
        float velo = PLAYER_VELOCITY * dt;

        if (this->Keys[GLFW_KEY_A]) {
            if (Player->position.x >= 0.0f) {
                Player->position.x -= velo;
                if (Ball->stuck) 
                    Ball->position.x -= velo;
            }
        }
        else if (this->Keys[GLFW_KEY_D]) {
            if (Player->position.x <= this->Width - Player->size.x) {
                Player->position.x += velo;
                if (Ball->stuck) 
                    Ball->position.x += velo;
            }
            
        }
        else if (this->Keys[GLFW_KEY_SPACE]) 
            Ball->stuck = false;
    }
}

void Game::Update(float dt)
{
    glm::vec2 startPos = Ball->position;
    glm::vec2 endPos = Ball->Move(dt, this->Width);

    Particles->Update(dt, *Ball, 1, glm::vec2(Ball->radius / 2.0f), (endPos != startPos));
    this->Collisions();
    if (shakeTime > 0.0f) {
        shakeTime -= dt;
        if (shakeTime <= 0.0f) {
            Post->m_shake = false;
        }
    }
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE) {
        Post->BindPostprocess();

        // Draw the background first!
        Renderer->Draw(ResourceManager::GetTexture("background"),
            glm::vec2(0.0f, 0.0f), 0.0f, glm::vec2(this->Width, this->Height));

        // Draw the player!
        Player->Draw(*Renderer);

        // Draw the rest of the sprites!
        this->levels[this->level].Draw(*Renderer);

        // Draw the particles!
        Particles->Draw();

        // Draw the Ball!
        Ball->Draw(*Renderer);

        Post->EndPostprocess();
        Post->PostprocessRender(glfwGetTime());
    }
}

void Game::Collisions()
{
    if (!Ball->stuck && CheckCircleCollision(*Player, *Ball)) {
        // Check where the ball hit the board
        float center = Player->position.x + (Player->size.x / 2.0f);
        float d = (Ball->position.x + Ball->radius) - center;
        float percent = d / (Player->size.x / 2.0f);

        // Change the ball's velocity accordingly.
        float strength = 2.0f;
        glm::vec2 oldVelo = Ball->velo;
        Ball->velo.x = INITIAL_BALL_VELOCITY.x * percent * strength;
        Ball->velo.y = -1.0f * glm::abs(Ball->velo.y);
        Ball->velo = glm::normalize(Ball->velo) * glm::length(oldVelo);
        return;
    }

    for (GameObject &b : this->levels[level].Bricks) {
        if (b.IsAlive() && CheckCircleCollision(b, *Ball)) {
            if (!b.IsSolid())
                b.Destroy();
            else {
                shakeTime = 0.05f;
                Post->m_shake = true;
            }
        }
    }
}