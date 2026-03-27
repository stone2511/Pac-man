#include "Ghost_Inky.hpp"


Ghost_Inky::Ghost_Inky(glm::vec2 worldPos) 
    : Ghost(RESOURCE_DIR"/Image/ghost/inky0.png", worldPos) {
}

void Ghost_Inky::Update(const Map& /* map */) {
    // TODO
    // auto pos = m_GhostObj->m_Transform.translation;
}
