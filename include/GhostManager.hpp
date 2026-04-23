#ifndef GHOST_MANAGER_HPP
#define GHOST_MANAGER_HPP

#include "Ghost.hpp"
#include "Map.hpp"
#include <vector>
#include <memory>

class GhostManager {
public:
    void Start(const Map& map);
    void Update(const Map& map, glm::vec2 pacmanPos, Direction pacmanDir);
    void Draw();
    void Reset();

    bool CheckCollision(glm::vec2 pacmanPos) const;

private:
    std::vector<std::shared_ptr<Ghost>> m_Ghosts;

    GhostState m_CurrentState = GhostState::SCATTER;

    //Timer
    float m_StateTimer = 0.0f;
    float m_RealseTimer = 0.0f;

    //鬼的釋放時間
    const std::vector<float> m_Re alse = {0.0f, 5.0f, 10.0f, 15.0f};
};

#endif
