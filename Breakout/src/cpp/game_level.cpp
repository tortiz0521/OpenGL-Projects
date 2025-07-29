#include "../headers/game_level.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

void GameLevel::Load(const char *file, unsigned int levelWidth, unsigned int levelHeight)
{
    this->Bricks.clear();
    unsigned int tileCode;
    ///aGameLevel level;

    std::string line;
    std::ifstream lfile(file);
    std::vector<std::vector<unsigned int>> tileData;
    if (lfile) {
        while (std::getline(lfile, line)) {
            std::stringstream stream(line);
            std::vector<unsigned int> row;
            while (stream >> tileCode) 
                row.push_back(tileCode);
            tileData.push_back(row);
        }

        if(tileData.size() > 0)
            this->init(tileData, levelWidth, levelHeight);
    }
}

void GameLevel::init(std::vector<std::vector<unsigned int>> tileData, unsigned int lw, unsigned int lh)
{
    // Calculate the dimenstions of the board first (NOTE: We are creating a coordinate system for our tiles!)
    unsigned int height = tileData.size();
    unsigned int width = tileData[0].size();
    float uHeight = lh / static_cast<float>(height);
    float uWidth = lw / static_cast<float>(width);
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // Figure out what type of block you have
            if (tileData[i][j] == 1) {
                this->Bricks.push_back(GameObject(
                    glm::vec2(uWidth * j, uHeight * i),
                    glm::vec2(uWidth, uHeight),
                    ResourceManager::GetTexture("block_solid"),
                    glm::vec2(0.0f, 0.0f),
                    glm::vec3(0.8f, 0.8f, 0.7f),
                    0.0f, true
                ));
            }
            else if (tileData[i][j] > 1) {
                glm::vec3 c = glm::vec3(1.0f); 
                if (tileData[i][j] == 2)
                    c = glm::vec3(0.2f, 0.6f, 1.0f);
                else if (tileData[i][j] == 3)
                    c = glm::vec3(0.0f, 0.7f, 0.0f);
                else if (tileData[i][j] == 4)
                    c = glm::vec3(0.8f, 0.8f, 0.4f);
                else if (tileData[i][j] == 5)
                    c = glm::vec3(1.0f, 0.5f, 0.0f);

                this->Bricks.push_back(GameObject(
                    glm::vec2(uWidth * j, uHeight * i),
                    glm::vec2(uWidth, uHeight),
                    ResourceManager::GetTexture("block"),
                    glm::vec2(0.0f, 0.0f),
                    c
                ));
            }
        }
    }
}

void GameLevel::Draw(SpriteRenderer &renderer)
{
    for (int i = 0; i < this->Bricks.size(); i++) {
        if (this->Bricks[i].IsAlive())
            this->Bricks[i].Draw(renderer);
    }
}

bool GameLevel::IsComplete()
{
    for (int i = 0; i < this->Bricks.size(); i++) {
        if (this->Bricks[i].IsAlive())
            return false;
    }

    return true;
}