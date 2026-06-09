#include "Ghost_Blinky.hpp"

Ghost_Blinky::Ghost_Blinky(glm::vec2 worldPos) 
    : Ghost("blinky", worldPos) {
}

void Ghost_Blinky::Update(const Map& map, glm::vec2 pacmanPos, Direction pacmanDir, glm::vec2 blinkyPos, GhostState state) {
    (void)pacmanDir;
    (void)blinkyPos;

    //Blinky 演算法是以玩家為target計算最短路徑
    glm::vec2 targetPos = {999.0f, 999.0f};
    if (state == GhostState::CHASE) {
        targetPos = pacmanPos;
    }

    UpdateMovement(map, targetPos, state);
}
