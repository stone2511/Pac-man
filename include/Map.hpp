#ifndef MAP_HPP
#define MAP_HPP

#include "pch.hpp"
#include "Util/GameObject.hpp"
#include <vector>
#include <memory>

class Map {
public:
    void Start();
    
    void Draw();

private:
    std::vector<std::vector<int>> m_Level;
    //Image of the wall
    std::vector<std::shared_ptr<Util::GameObject>> m_Blocks;
};

#endif