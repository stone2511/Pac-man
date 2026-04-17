#include "Ghost_Clyde.hpp"


Ghost_Clyde::Ghost_Clyde(glm::vec2 worldPos) 
    : Ghost("Clyde", RESOURCE_DIR"/Image/ghost/clyde0.png", 'C', worldPos) {
}

void Ghost_Clyde::Update(const Map& map, glm::vec2 pacmanPos, GhostState state) {
    (void)pacmanPos;
    (void)state;

    if (UpdateHouseRelease()) {
        return;
    }

    UpdateRandomMovement(map);
}
