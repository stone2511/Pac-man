#include "Map.hpp"
#include "Util/Logger.hpp"
#include "Util/Image.hpp"
#include <imgui.h>
#include <string>
#include <cmath>
#include <utility>

void Map::Start() {
    m_Blocks.clear();
    m_dots.clear();
    m_dotplus.clear();
    m_door.clear();

    //Map
    m_Level = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3, 1},
        {1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1},
        {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
        {1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1},
        {1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1},
        {1, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 2, 1, 0, 1, 1, 4, 1, 1, 0, 1, 2, 1, 1, 1, 1, 1},
        {2, 2, 2, 2, 2, 2, 0, 0, 1, 0, 0, 0, 1, 0, 0, 2, 2, 2, 2, 2, 2}, //tunnel
        {1, 1, 1, 1, 1, 2, 1, 0, 1, 1, 1, 1, 1, 0, 1, 2, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1},
        {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
        {1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1},
        {1, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 1},
        {1, 1, 1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 1, 1},
        {1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1},
        {1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1},
        {1, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    //21*32
    float mapWidth = m_Level[0].size() * m_GridSize; 
    //21*32
    float mapHeight = m_Level.size() * m_GridSize;

    m_StartX = -(mapWidth / 2.0f) + (m_GridSize / 2.0f); 
    m_StartY = (mapHeight / 2.0f) - (m_GridSize / 2.0f);

    for (int y = 0; y < m_Level.size(); y++) {
        for (int x = 0; x < m_Level[y].size(); x++) {
            

            if (m_Level[y][x] == 1) {
                //I use binary method to encode the image name
                //And use wallID to pair the correct wall texture
                bool up    = IsWallOrEdge(x, y - 1);
                bool down  = IsWallOrEdge(x, y + 1);
                bool left  = IsWallOrEdge(x - 1, y);
                bool right = IsWallOrEdge(x + 1, y);
                
                int wallID = 0;
                if (up)     wallID += 8;
                if (down)   wallID += 4;
                if (left)   wallID += 2;
                if (right)  wallID += 1;

                static const std::string wallTextures[16] = {
                    RESOURCE_DIR"/Image/backround/wall0000.png",
                    RESOURCE_DIR"/Image/backround/wall0001.png",
                    RESOURCE_DIR"/Image/backround/wall0010.png",
                    RESOURCE_DIR"/Image/backround/wall0011.png",
                    RESOURCE_DIR"/Image/backround/wall0100.png",
                    RESOURCE_DIR"/Image/backround/wall0101.png",
                    RESOURCE_DIR"/Image/backround/wall0110.png",
                    RESOURCE_DIR"/Image/backround/wall0111.png",
                    RESOURCE_DIR"/Image/backround/wall1000.png",
                    RESOURCE_DIR"/Image/backround/wall1001.png",
                    RESOURCE_DIR"/Image/backround/wall1010.png",
                    RESOURCE_DIR"/Image/backround/wall1011.png",
                    RESOURCE_DIR"/Image/backround/wall1100.png",
                    RESOURCE_DIR"/Image/backround/wall1101.png",
                    RESOURCE_DIR"/Image/backround/wall1110.png",
                };

                auto block = std::make_shared<Util::GameObject>();

                block->SetDrawable(std::make_shared<Util::Image>(wallTextures[wallID]));
    
                block->m_Transform.translation = {m_StartX + (x * m_GridSize), m_StartY - (y * m_GridSize)};
                block->SetZIndex(0);
                m_Blocks.push_back(block);
            }

            else if (m_Level[y][x] == 2) {
                auto dot = std::make_shared<Util::GameObject>();
            
                dot->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/Image/backround/dot.png"));
                
                dot->m_Transform.translation = {m_StartX + (x * m_GridSize), m_StartY - (y * m_GridSize)};
                dot->SetZIndex(0);
                
                m_dots.push_back(dot);
            }

            else if (m_Level[y][x] == 3) {
                auto dotplus = std::make_shared<Util::GameObject>();
            
                dotplus->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/Image/backround/dotplus.png"));
                
                dotplus->m_Transform.translation = {m_StartX + (x * m_GridSize), m_StartY - (y * m_GridSize)};
                dotplus->SetZIndex(0);
                
                m_dotplus.push_back(dotplus);
            }

            else if (m_Level[y][x] == 4) {
                auto door = std::make_shared<Util::GameObject>();
            
                door->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/Image/backround/door.png"));
                
                door->m_Transform.translation = {m_StartX + (x * m_GridSize), m_StartY - (y * m_GridSize)};
                door->SetZIndex(0);
                
                m_door.push_back(door);
            }
        }
    }

}

void Map::Draw() {
    for (auto& block : m_Blocks) {
        block->Draw();
    }
    for (auto& dot : m_dots) {
        dot->Draw();
    }
    for (auto& dotplus : m_dotplus) {
        dotplus->Draw();
    }
    for (auto& door : m_door) {
        door->Draw();
    }
}

bool Map::IsWall(float x, float y) const {

    int gridX = std::round((x - m_StartX) / m_GridSize);
    int gridY = std::round((m_StartY - y) / m_GridSize);

    
    int maxX = m_Level[0].size() - 1;
    
    
    if (gridX <= 0 || gridX >= maxX) {
        
        if (gridY != 9) {
            return true;
        }
    }


    return IsWallOrEdge(gridX, gridY);
}

bool Map::IsDoor(float x, float y) const {
    int gridX = static_cast<int>((x - m_StartX + (m_GridSize / 2.0f)) / m_GridSize);
    int gridY = static_cast<int>((m_StartY - y + (m_GridSize / 2.0f)) / m_GridSize);

    return (m_Level[gridY][gridX] == 4);
}

bool Map::IsWallOrEdge(int gridX, int gridY) const {
    if (gridY < 0 || gridY >= m_Level.size() || gridX < 0 || gridX >= m_Level[0].size()) {
        return false;
    }

    return m_Level[gridY][gridX] == 1;
}

glm::vec2 Map::GridToWorld(float gridX, float gridY) const {
    return {
        m_StartX + (gridX * m_GridSize),
        m_StartY - (gridY * m_GridSize)
    };
}

bool Map::IsLevelClear() const { 
    return m_dots.empty();
}

void Map::ResetData(){
    m_dots.clear();

    for (int y = 0; y < m_Level.size(); y++) {
        for (int x = 0; x < m_Level[y].size(); x++) {
            if (m_Level[y][x] == 2) {
                auto dot = std::make_shared<Util::GameObject>();
            
                dot->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/Image/backround/dot.png"));
                
                dot->m_Transform.translation = {m_StartX + (x * m_GridSize), m_StartY - (y * m_GridSize)};
                dot->SetZIndex(0);
                
                m_dots.push_back(dot);
            }
        }
    }
}

int Map::CheckAndEatBeans(glm::vec2 pacmanPos) {
    int scoreToGive = 0;
    float eatRadius = 15.0f;

    for (auto it = m_dots.begin(); it != m_dots.end(); ) {
        glm::vec2 dotPos = (*it)->m_Transform.translation;
        
        float distX = std::abs(pacmanPos.x - dotPos.x);
        float distY = std::abs(pacmanPos.y - dotPos.y);

        if (distX < eatRadius && distY < eatRadius) {
            it = m_dots.erase(it); 
            scoreToGive += 10;
        } else {
            ++it; 
        }
    }
    
    return scoreToGive;
}

bool Map::TryWrapTunnel(glm::vec2& pos, float radius) const {
    if (m_Level.empty()) {
        return false;
    }

    constexpr int tunnelRow = 9;

    if (tunnelRow < 0 || tunnelRow >= static_cast<int>(m_Level.size())) {
        return false;
    }

    const float tunnelY = m_StartY - (tunnelRow * m_GridSize);
    const float tunnelTolerance = m_GridSize / 2.0f;
    if (std::abs(pos.y - tunnelY) > tunnelTolerance) {
        return false;
    }

    const float leftEdge = m_StartX - (m_GridSize / 2.0f);
    const float rightEdge =
        m_StartX + (static_cast<float>(m_Level[0].size()) * m_GridSize) -
        (m_GridSize / 2.0f);
    const float leftEntranceX = m_StartX;
    const float rightEntranceX =
        m_StartX + (static_cast<float>(m_Level[0].size() - 1) * m_GridSize);

    if (pos.x - radius <= leftEdge) {
        pos.x = rightEntranceX;
        return true;
    }

    if (pos.x + radius >= rightEdge) {
        pos.x = leftEntranceX;
        return true;
    }

    return false;
}

glm::vec2 Map::GetClosestGridCenter(float x, float y) const {

    int gridX = std::round((x - m_StartX) / m_GridSize);
    int gridY = std::round((m_StartY - y) / m_GridSize);

    int maxX = m_Level[0].size() - 1;
    int maxY = m_Level.size() - 1;
    
    gridX = std::clamp(gridX, 0, maxX);
    gridY = std::clamp(gridY, 0, maxY);

    float centerX = m_StartX + (gridX * m_GridSize);
    float centerY = m_StartY - (gridY * m_GridSize);

    return glm::vec2(centerX, centerY);
}

