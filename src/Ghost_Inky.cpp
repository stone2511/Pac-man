#include "Ghost_Inky.hpp"

Ghost_Inky::Ghost_Inky(glm::vec2 worldPos) 
    : Ghost(RESOURCE_DIR"/Image/ghost/inky0.png", worldPos) {
}

void Ghost_Inky::Update(const Map& map, glm::vec2 pacmanPos, Direction pacmanDir, glm::vec2 blinkyPos, GhostState state) {
    glm::vec2 targetPos;

    if (state == GhostState::CHASE) {
        float offset = 2.0f * 32.0f;
        glm::vec2 privotPos = pacmanPos;
        
        if(pacmanDir == Direction::UP) {privotPos.y+=offset;}
        else if(pacmanDir == Direction::DOWN) {privotPos.y-=offset;}
        else if(pacmanDir == Direction::LEFT) {privotPos.x-=offset;}
        else if(pacmanDir == Direction::RIGHT) {privotPos.x+=offset;}

        glm::vec2 vectorFromBlinky = privotPos - blinkyPos;
        targetPos = blinkyPos + (vectorFromBlinky * 2.0f);
            
    } else if (state == GhostState::SCATTER) {
        targetPos = glm::vec2(999.0f, -999.0f); 
    } else {
        targetPos = pacmanPos;
    }

    UpdateMovement(map, targetPos, state);
}
