#include "Ghost_Inky.hpp"

Ghost_Inky::Ghost_Inky(glm::vec2 worldPos) 
    : Ghost("Inky", RESOURCE_DIR"/Image/ghost/inky0.png", 'I', worldPos) {
}

void Ghost_Inky::Update(const Map& map, glm::vec2 pacmanPos, GhostState state) {
    (void)pacmanPos;
    (void)state;

    if (UpdateHouseRelease()) {
        return;
    }

    UpdateRandomMovement(map);
}
