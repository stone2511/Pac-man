#ifndef GHOST_BLINKY_HPP
#define GHOST_BLINKY_HPP

#include "Ghost.hpp"


class Ghost_Blinky : public Ghost {
public:
    explicit Ghost_Blinky(glm::vec2 worldPos);

    void Update(const Map& map, glm::vec2 pacmanPos, GhostState state) override;
};

#endif
