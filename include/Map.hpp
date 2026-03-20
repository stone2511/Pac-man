#ifndef MAP_HPP
#define MAP_HPP

#include "pch.hpp"
#include "Util/Image.hpp"
#include <vector>
#include <memory>

class Map {
public:
    void Start();
    
    void Draw();

private:
    std::vector<std::vector<int>> m_Grid; 
    
    //std::shared_ptr<Util::Image> m_WallImage;
    //std::shared_ptr<Util::Image> m_CoinImage;
};

#endif