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
    bool IsDoor(float x, float y) const;

    //For the wall
    bool IsWallOrEdge(int gridX, int gridY) const;

    int CheckAndEatBeans(glm::vec2 pacmanPos);
    bool TryWrapTunnel(glm::vec2& pos, float radius) const;

private:
    glm::vec2 GridToWorld(float gridX, float gridY) const;

    std::vector<std::vector<int>> m_Level;
    
    std::vector<std::shared_ptr<Util::GameObject>> m_Blocks;
    std::vector<std::shared_ptr<Util::GameObject>> m_dots;
    std::vector<std::shared_ptr<Util::GameObject>> m_dotplus;
    std::vector<std::shared_ptr<Util::GameObject>> m_door;
    std::vector<std::shared_ptr<Util::GameObject>> m_Ghosts;

    float m_GridSize = 32.0f;
    float m_StartX;
    float m_StartY;
};

#endif
