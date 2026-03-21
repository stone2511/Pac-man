#ifndef MAP_HPP
#define MAP_HPP

#include "pch.hpp"
#include "Util/GameObject.hpp"
#include <vector>
#include <memory>

class Map {
public:
    void Start();
    
    void Draw();

    bool IsWall(float x, float y) const;

    //For the wall
    bool IsWallOrEdge(int gridX, int gridY) const;

private:
    std::vector<std::vector<int>> m_Level;
    
    std::vector<std::shared_ptr<Util::GameObject>> m_Blocks;
    std::vector<std::shared_ptr<Util::GameObject>> m_dots;
    std::vector<std::shared_ptr<Util::GameObject>> m_dotplus;

    float m_GridSize = 32.0f;
    float m_StartX;
    float m_StartY;
};

#endif