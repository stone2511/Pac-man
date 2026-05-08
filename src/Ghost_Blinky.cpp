#include "Ghost_Blinky.hpp"
Ghost_Blinky::Ghost_Blinky(glm::vec2 worldPos) 
    : Ghost(RESOURCE_DIR"/Image/ghost/blinky0.png", worldPos) {
}

void Ghost_Blinky::Update(const Map& map, glm::vec2 pacmanPos, Direction pacmanDir, glm::vec2 blinkyPos, GhostState state) {
    (void)pacmanDir;
    (void)blinkyPos;

    glm::vec2 targetPos;

    if (state == GhostState::SCATTER) {
        targetPos = glm::vec2(999.0f, 999.0f); 
    } else {
        targetPos = pacmanPos;
    }

    UpdateMovement(map, targetPos, state);
}
