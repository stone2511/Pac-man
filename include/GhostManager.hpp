#ifndef GHOST_MANAGER_HPP
#define GHOST_MANAGER_HPP

#include "Ghost.hpp"
#include "Map.hpp"
#include <vector>
#include <memory>

class GhostManager {
public:
    void Start(const Map& map);
    void Update(const Map& map);
    void Draw();

private:
    std::vector<std::shared_ptr<Ghost>> m_Ghosts;
};

#endif