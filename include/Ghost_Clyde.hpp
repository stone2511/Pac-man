#ifndef GHOST_CLYDE_HPP
#define GHOST_CLYDE_HPP

#include "Ghost.hpp"


class Ghost_Clyde : public Ghost {
public:
    explicit Ghost_Clyde(glm::vec2 worldPos);

    void Update(const Map& map, glm::vec2 pacmanPos, Direction pacmanDir, glm::vec2 blinkyPos, GhostState state) override; 

private:
    
    //bool key =  false;
    Direction m_CurrentDir = Direction::LEFT;
};

#endif
