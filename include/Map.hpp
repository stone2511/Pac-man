#ifndef MAP_HPP
#define MAP_HPP

#include "pch.hpp"
#include "Util/GameObject.hpp"
#include <vector>
#include <memory>

struct BeanEatResult {
    int score = 0;
    bool atePowerPellet = false;
};

class Map {
public:
    void Start();
    
    void Draw();

    bool IsWall(float x, float y) const;
    bool IsDoor(float x, float y) const;

    //For the wall
    bool IsWallOrEdge(int gridX, int gridY) const;

    BeanEatResult CheckAndEatBeans(glm::vec2 pacmanPos);
    bool TryWrapTunnel(glm::vec2& pos, float radius) const;

    bool IsLevelClear() const;
    void ResetData();
    
    glm::vec2 GridToWorld(float gridX, float gridY) const;
    glm::vec2 GetClosestGridCenter(float x, float y) const;

private:
    std::vector<std::vector<int>> m_Level;
    
    std::vector<std::shared_ptr<Util::GameObject>> m_Blocks;
    std::vector<std::shared_ptr<Util::GameObject>> m_dots;
    std::vector<std::shared_ptr<Util::GameObject>> m_dotplus;
    std::vector<std::shared_ptr<Util::GameObject>> m_door;

    float m_GridSize = 32.0f;
    float m_StartX;
    float m_StartY;
};

#endif
