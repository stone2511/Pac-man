#ifndef GHOST_PINKY_HPP
#define GHOST_PINKY_HPP

#include "Ghost.hpp"


class Ghost_Pinky : public Ghost {
public:
    explicit Ghost_Pinky(glm::vec2 worldPos);

    void Update(const Map& map) override; 
};

#endif
