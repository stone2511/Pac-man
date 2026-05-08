#include "Ghost_Clyde.hpp"

Ghost_Clyde::Ghost_Clyde(glm::vec2 worldPos) 
    : Ghost("clyde", worldPos) {
}

void Ghost_Clyde::Update(const Map& map, glm::vec2 pacmanPos, Direction pacmanDir, glm::vec2 blinkyPos, GhostState state) {
    (void)pacmanDir;
    (void)blinkyPos;

    const glm::vec2 pos = m_GhostObj->m_Transform.translation;
    glm::vec2 targetPos = {-999.0f, -999.0f};
    if (state == GhostState::CHASE) {
        const float distance = glm::distance(pos, pacmanPos);
        const float switchDistance = 8.0f * 32.0f;

        if (distance > switchDistance) {
            targetPos = pacmanPos;
        }
    }

    UpdateMovement(map, targetPos, state);
}
