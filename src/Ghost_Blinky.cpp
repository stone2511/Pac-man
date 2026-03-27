#include "Ghost_Blinky.hpp"


Ghost_Blinky::Ghost_Blinky(glm::vec2 worldPos) 
    : Ghost(RESOURCE_DIR"/Image/ghost/blinky0.png", worldPos) {
}

void Ghost_Blinky::Update(const Map& /* map */) {
    // TODO
    // auto pos = m_GhostObj->m_Transform.translation;
}
