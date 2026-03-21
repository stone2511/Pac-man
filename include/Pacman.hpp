#ifndef PACMAN_HPP
#define PACMAN_HPP

#include "pch.hpp"
#include "Util/GameObject.hpp"
#include <memory>

class Pacman {
public:
    void Start();
    void Update();
    void Draw();

    //Collision Detect
    glm::vec2 GetPosition() const;
private:
    std::shared_ptr<Util::GameObject> m_Pacman;
    //Pacman Speed
    float m_Speed = 4.0f;
};

#endif