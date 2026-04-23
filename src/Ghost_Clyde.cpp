#include "Ghost_Clyde.hpp"


Ghost_Clyde::Ghost_Clyde(glm::vec2 worldPos) 
    : Ghost("Clyde", RESOURCE_DIR"/Image/ghost/clyde0.png", 'C', worldPos) {
}

void Ghost_Clyde::Update(const Map& map,
                         glm::vec2 pacmanPos,
                         Direction pacmanDir,
                         glm::vec2 blinkyPos,
                         GhostState state) {
    (void)pacmanDir;
    (void)blinkyPos;
    (void)pacmanPos;
    (void)state;

    //出門邏輯
    if(m_HouseState == HouseState::EXITING){
        glm::vec2 doorPos = map.GridToWorld(10,7);
        float exitSpeed = 2.0f;

        if(std::abs(pos.x - doorPos.x) > exitSpeed){
            pos.x += (pos.x < doorPos.x) ? exitSpeed : -exitSpeed;
        }
        else{
            pos.x = doorPos.x;
            if(std::abs(pos.y - doorPos.y) > exitSpeed){
                pos.y += (pos.y < doorPos.y) ? exitSpeed : -exitSpeed;
            }
            else{
                pos.y = doorPos.y;
                m_HouseState = HouseState::OUTSIDE;
                m_CurrentDir = Direction::LEFT;
            }
        }

        m_GhostObj->m_Transform.translation = pos;
        return;
    }

    UpdateRandomMovement(map);
}
