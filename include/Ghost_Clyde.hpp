#ifndef GHOST_CLYDE_HPP
#define GHOST_CLYDE_HPP

#include "Ghost.hpp"


class Ghost_Clyde : public Ghost {
public:
    Clyde(glm::vec2 worldPos);

    void Update(const Map& map) override; 
};

#endif