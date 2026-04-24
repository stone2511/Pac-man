#include "Ghost_Clyde.hpp"


Ghost_Clyde::Ghost_Clyde(glm::vec2 worldPos) 
    : Ghost(RESOURCE_DIR"/Image/ghost/clyde0.png", worldPos) {
}

void Ghost_Clyde::Update(const Map& map, glm::vec2 pacmanPos, Direction pacmanDir, glm::vec2 blinkyPos, GhostState state) {
    (void)pacmanDir;
    (void)blinkyPos;

    glm::vec2 targetPos;

    if (state == GhostState::CHASE) {
        const glm::vec2 pos = m_GhostObj->m_Transform.translation;
        float distance = glm::distance(pos, pacmanPos);
        float D_distance = 8.0f * 32.0f;

        if(distance > D_distance){
            targetPos = pacmanPos;
        }
        else{
            targetPos = glm::vec2(-999.0f, -999.0f);
        }
    } else if (state == GhostState::SCATTER) {
        targetPos = glm::vec2(-999.0f, -999.0f); 
    } else {
        targetPos = pacmanPos;
    }

    UpdateMovement(map, targetPos, state);
}
