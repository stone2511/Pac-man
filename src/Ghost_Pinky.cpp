#include "Ghost_Pinky.hpp"


Ghost_Pinky::Ghost_Pinky(glm::vec2 worldPos) 
    : Ghost(RESOURCE_DIR"/Image/ghost/pinky0.png", worldPos) {
}

void Ghost_Pinky::Update(const Map& map, glm::vec2 pacmanPos, Direction pacmanDir, glm::vec2 blinkyPos, GhostState state) {
    (void)blinkyPos;

    glm::vec2 targetPos;

    if (state == GhostState::CHASE) {
        float offset = 4.0f * 32.0f;
        targetPos = pacmanPos;
        
        if(pacmanDir == Direction::UP) {targetPos.y+=offset;}
        else if(pacmanDir == Direction::DOWN) {targetPos.y-=offset;}
        else if(pacmanDir == Direction::LEFT) {targetPos.x-=offset;}
        else if(pacmanDir == Direction::RIGHT) {targetPos.x+=offset;}
            
    } else if (state == GhostState::SCATTER) {
        targetPos = glm::vec2(-999.0f, 999.0f); 
    } else {
        targetPos = pacmanPos;
    }

    UpdateMovement(map, targetPos, state);
}
