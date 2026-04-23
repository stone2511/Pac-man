#include "Ghost_Pinky.hpp"


Ghost_Pinky::Ghost_Pinky(glm::vec2 worldPos) 
    : Ghost("Pinky", RESOURCE_DIR"/Image/ghost/pinky0.png", 'P', worldPos) {
}

void Ghost_Pinky::Update(const Map& map, glm::vec2 pacmanPos, GhostState state) {
    (void)pacmanPos;
    (void)state;

    if (UpdateHouseRelease()) {
        return;
    }

    UpdateRandomMovement(map);
}
