#ifndef GHOST_MANAGER_HPP
#define GHOST_MANAGER_HPP

#include "Ghost.hpp"
#include "Map.hpp"
#include <vector>
#include <memory>

enum class GhostCollisionResult {
    NONE,
    PACMAN_DIED,
    GHOST_EATEN
};

class GhostManager {
public:
    void Start(const Map& map);
    void Update(const Map& map, glm::vec2 pacmanPos, Direction pacmanDir);
    void Draw();
    void Reset();
    void SetVisible(bool visible);
    void TriggerPowerMode();
    int GetGhostEatScore() const;

    GhostCollisionResult ResolveCollision(glm::vec2 pacmanPos);

private:
    std::vector<std::shared_ptr<Ghost>> m_Ghosts;

    GhostState m_CurrentState = GhostState::SCATTER;
    GhostState m_NormalState = GhostState::SCATTER;

    //Timer
    float m_StateTimer = 0.0f;
    float m_ReleaseTimer = 0.0f;
    float m_FrightenedTimer = 0.0f;
    int m_GhostEatChain = 0;

    //鬼的釋放時間
    const std::vector<float> m_Release = {0.0f, 5.0f, 10.0f, 15.0f};
    const float m_FrightenedDuration = 8.0f;
};

#endif
