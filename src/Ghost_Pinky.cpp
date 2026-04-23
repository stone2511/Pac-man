#include "Ghost_Pinky.hpp"


Ghost_Pinky::Ghost_Pinky(glm::vec2 worldPos) 
    : Ghost("Pinky", RESOURCE_DIR"/Image/ghost/pinky0.png", 'P', worldPos) {
}

void Ghost_Pinky::Update(const Map& map,
                         glm::vec2 pacmanPos,
                         Direction pacmanDir,
                         glm::vec2 blinkyPos,
                         GhostState state) {
    (void)pacmanDir;
    (void)blinkyPos;
    (void)pacmanPos;
    (void)state;

    if (UpdateHouseRelease()) {
        return;
    }

    UpdateRandomMovement(map);
}
