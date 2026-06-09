#include "Ghost_Pinky.hpp"

Ghost_Pinky::Ghost_Pinky(glm::vec2 worldPos) 
    : Ghost("pinky", worldPos) {
    SetHouseIdleStartDirection(Direction::DOWN);
}

void Ghost_Pinky::Update(const Map& map, glm::vec2 pacmanPos, Direction pacmanDir, glm::vec2 blinkyPos, GhostState state) {
    (void)blinkyPos;

    //Pinky 演算法是以玩家面向的前方4個單位為target計算最短路徑
    glm::vec2 targetPos = {-999.0f, 999.0f};
    if (state == GhostState::CHASE) {
        const float offset = 4.0f * 32.0f;
        targetPos = pacmanPos;

        if (pacmanDir == Direction::UP) { targetPos.y += offset; }
        else if (pacmanDir == Direction::DOWN) { targetPos.y -= offset; }
        else if (pacmanDir == Direction::LEFT) { targetPos.x -= offset; }
        else if (pacmanDir == Direction::RIGHT) { targetPos.x += offset; }
    }

    UpdateMovement(map, targetPos, state);
}
