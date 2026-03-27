#include "Ghost_Pinky.hpp"


Ghost_Pinky::Ghost_Pinky(glm::vec2 worldPos) 
    : Ghost(RESOURCE_DIR"/Image/ghost/pinky0.png", worldPos) {
}

void Ghost_Pinky::Update(const Map& map, glm::vec2 pacmanPos, GhostState state) {
    // TODO
    // auto pos = m_GhostObj->m_Transform.translation;
}
