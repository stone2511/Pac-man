#ifndef PACMAN_HPP
#define PACMAN_HPP

#include "pch.hpp"
#include "Util/GameObject.hpp"
#include "Map.hpp"
#include <memory>

class Pacman {
public:
    void Start();   //Init
    int Update(Map& map);    //Movement and Behavior
    void Draw();    //Draw

private:
    enum class Direction {
        None,
        Up,
        Down,
        Left,
        Right
    };

    std::shared_ptr<Util::GameObject> m_Pacman;
    //Pacman Speed
    float m_Speed = 4.0f;
    Direction m_CurrentDirection = Direction::None;
    Direction m_QueuedDirection = Direction::None;

    //Colliging Detect
    bool IsColliding(Map& map, glm::vec2 pos);
    glm::vec2 GetDirectionOffset(Direction direction) const;
};

#endif
