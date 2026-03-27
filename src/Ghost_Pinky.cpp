#include "Ghost_Pinky.hpp"


Ghost_Pinky::Pinky(glm::vec2 worldPos) 
    : Ghost(RESOURCE_DIR"/Image/ghost/pinky0.png", worldPos) {
}

void Ghost_Pinky::Update(const Map& map) {
    // TODO
    // auto pos = m_GhostObj->m_Transform.translation;
}