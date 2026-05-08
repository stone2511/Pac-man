#ifndef PACMAN_HPP
#define PACMAN_HPP

#include "pch.hpp"
#include "Util/GameObject.hpp"
#include "Util/Animation.hpp"
#include "Map.hpp"
#include "Direction.hpp"
#include <memory>

class Pacman {
public:
    void Start();   //Init
    BeanEatResult Update(Map& map);    //Movement and behavior
    void Animate();
    void Draw();    //Draw
    void PauseAnimation();
    void StartDeathAnimation();
    bool IsDeathAnimationFinished() const;
    void Reset();

    glm::vec2 GetPosition() const;
    Direction GetDirection() const;

private:
    
    std::shared_ptr<Util::GameObject> m_Pacman;

    //動畫
    std::shared_ptr<Util::Animation> m_UpAnimation;
    std::shared_ptr<Util::Animation> m_DownAnimation;
    std::shared_ptr<Util::Animation> m_LeftAnimation;
    std::shared_ptr<Util::Animation> m_RightAnimation;
    std::shared_ptr<Util::Animation> m_DeathAnimation;
    std::shared_ptr<Util::Animation> m_CurrentAnimation;

    //Pacman Speed
    float m_Speed = 4.0f;

    Direction m_CurrentDirection = Direction::NONE;
    Direction m_QueuedDirection = Direction::NONE;
    Direction m_FacingDirection = Direction::RIGHT;

    //Colliging Detect
    bool IsColliding(Map& map, glm::vec2 pos);

    glm::vec2 GetDirectionOffset(Direction direction) const;

    std::shared_ptr<Util::Animation> GetAnimation(Direction direction) const;
    void UpdateAnimation(bool isMoving);
};

#endif
