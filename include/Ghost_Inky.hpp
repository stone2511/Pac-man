#ifndef GHOST_INKY_HPP
#define GHOST_INKY_HPP

#include "Ghost.hpp"


class Ghost_Inky : public Ghost {
public:
    explicit Ghost_Inky(glm::vec2 worldPos);

    void Update(const Map& map) override; 
};

#endif
