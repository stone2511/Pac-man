#ifndef PACMAN_HPP
#define PACMAN_HPP

#include "pch.hpp"
#include "Util/GameObject.hpp"
#include "Map.hpp"
#include <memory>

class Pacman {
public:
    void Start();   //Init
    int Update(Map& map);    //Movement and behavior
    void Draw();    //Draw

private:
    std::shared_ptr<Util::GameObject> m_Pacman;
    //Pacman Speed
    float m_Speed = 4.0f;

    //Radius of the entity
    float m_radius = 14.0f;

    //Colliging Detect
    bool IsColliding(Map& map, glm::vec2 pos);
};

#endif