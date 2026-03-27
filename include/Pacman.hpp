#ifndef PACMAN_HPP
#define PACMAN_HPP

#include "pch.hpp"
#include "Util/GameObject.hpp"
#include "Util/Animation.hpp"
#include "Map.hpp"
#include <memory>

class Pacman {
public:
    void Start();   //Init
    void Animate();
    int Update(Map& map);    //Movement and Behavior
    void Draw();    //Draw

    glm::vec2 GetPosition() const;

private:
    enum class Direction {
        None,
        Up,
        Down,
        Left,
        Right
    };

    std::shared_ptr<Util::GameObject> m_Pacman;

    std::shared_ptr<Util::Animation> m_UpAnimation;
    std::shared_ptr<Util::Animation> m_DownAnimation;
    std::shared_ptr<Util::Animation> m_LeftAnimation;
    std::shared_ptr<Util::Animation> m_RightAnimation;
    std::shared_ptr<Util::Animation> m_CurrentAnimation;

    //Pacman Speed
    float m_Speed = 4.0f;

    Direction m_CurrentDirection = Direction::None;
    Direction m_QueuedDirection = Direction::None;
    Direction m_FacingDirection = Direction::Right;

    //Colliging Detect
    bool IsColliding(Map& map, glm::vec2 pos);

    glm::vec2 GetDirectionOffset(Direction direction) const;

    std::shared_ptr<Util::Animation> GetAnimation(Direction direction) const;
    void UpdateAnimation(bool isMoving);
};

#endif
