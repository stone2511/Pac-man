#ifndef PACMAN_HPP
#define PACMAN_HPP

#include "pch.hpp"
#include "Util/GameObject.hpp"
#include "Map.hpp"
#include <memory>

class Pacman {
public:
    void Start();   //Init
    void Update(const Map& map);    //Movement and Behavior
    void Draw();    //Draw

private:
    std::shared_ptr<Util::GameObject> m_Pacman;
    //Pacman Speed
    float m_Speed = 4.0f;

    //Colliging Detect
    bool IsColliding(const Map& map, glm::vec2 pos);
};

#endif