#include "Map.hpp"
#include "Util/Logger.hpp"
#include "Util/Image.hpp"
#include <imgui.h>
#include <string>

void Map::Start() {
    //Map
    m_Level = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
        {1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1},
        {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
        {1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1},
        {1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1},
        {1, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 2, 1, 0, 1, 1, 0, 1, 1, 0, 1, 2, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 0, 0}, // 👈 左右相通的隧道
        {1, 1, 1, 1, 1, 2, 1, 0, 1, 1, 1, 1, 1, 0, 1, 2, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1},
        {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
        {1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1},
        {1, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 1},
        {1, 1, 1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 1, 1},
        {1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1},
        {1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1},
        {1, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
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
                auto block = std::make_shared<Util::GameObject>();
            
                block->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/Image/backround/wall.png"));
                
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
                //todo
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
}

bool Map::IsWall(float x, float y) const {

    int gridX = static_cast<int>((x - m_StartX + (m_GridSize / 2.0f)) / m_GridSize);
    int gridY = static_cast<int>((m_StartY - y + (m_GridSize / 2.0f)) / m_GridSize);

    
    if (gridY < 0 || gridY >= m_Level.size() || gridX < 0 || gridX >= m_Level[0].size()) {
        return true;
    }

    return m_Level[gridY][gridX] == 1;
}