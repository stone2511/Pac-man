#include "Ghost_Clyde.hpp"


Ghost_Clyde::Ghost_Clyde(glm::vec2 worldPos) 
    : Ghost(RESOURCE_DIR"/Image/ghost/clyde0.png", worldPos) {
}

void Ghost_Clyde::Update(const Map& map, glm::vec2 pacmanPos, GhostState state) {
    // TODO
    // auto pos = m_GhostObj->m_Transform.translation;
}
