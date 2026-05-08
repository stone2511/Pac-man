#ifndef GHOST_HPP
#define GHOST_HPP

#include "pch.hpp"
#include "Util/GameObject.hpp"
#include "Util/Animation.hpp"
#include "Map.hpp"
#include "Direction.hpp"
#include <memory>
#include <string>
#include <vector>

enum class GhostState {
        SCATTER,
        CHASE,
        FRIGHTENED,
        EATEN
};

enum class HouseState {
        IN_HOUSE,
        EXITING,
        OUTSIDE,
        RETURNING
};


class Ghost {
public:
    Ghost(const std::string& texturePath, glm::vec2 worldPos);
    virtual ~Ghost() = default;

    virtual void Update(const Map& map, glm::vec2 pacmanPos, Direction pacmanDir, glm::vec2 blinkyPos, GhostState state) = 0; 
    
    void Draw();
    glm::vec2 GetPosition() const;

    void Reset();
    void BecomeEaten();
    void ReverseDirection();
    void SetFrightenedTimeRemaining(float timeRemaining);
    void ResetFrightenedImmunity();
    bool IgnoresFrightened() const;
    bool IsReturningToHouse() const;

    void SetIsActive(bool active);
    bool IsActive() const;
    void SetVisible(bool visible);
    bool IsVisible() const;

    void SetHouseState(HouseState s);
    HouseState GetHouseState() const;
    void SetHomePosition(glm::vec2 homePos);

protected:
    void UpdateMovement(const Map& map, glm::vec2 targetPos, GhostState state);
    void UpdateDrawableForState(GhostState state);
    bool HandleExitHouse(const Map& map, glm::vec2& pos);
    bool HandleReturnToHouse(const Map& map, glm::vec2& pos);
    Direction ChooseNextDirection(const Map& map, glm::vec2 pos, glm::vec2 targetPos, GhostState state) const;
    Direction ChooseRandomDirection(const Map& map, glm::vec2 pos, Direction previousDirection) const;
    std::vector<Direction> GetValidDirections(const Map& map, glm::vec2 pos) const;
    bool CanMoveDirection(const Map& map, glm::vec2 pos, Direction direction, float distance) const;
    static glm::vec2 GetDirectionVector(Direction direction, float speed);
    static Direction OppositeDirection(Direction direction);

    glm::vec2 m_SpawnPos;
    glm::vec2 m_HomePos;

    bool m_IsActive = false;

    HouseState m_HouseState = HouseState::IN_HOUSE;

protected:
    std::shared_ptr<Util::GameObject> m_GhostObj;
    std::shared_ptr<Core::Drawable> m_NormalDrawable;
    std::shared_ptr<Core::Drawable> m_EyesDrawable;
    std::shared_ptr<Util::Animation> m_FrightenedAnimation;
    std::shared_ptr<Util::Animation> m_FrightenedWarningAnimation;
    float m_NormalSpeed = 2.0f;
    float m_FrightenedSpeed = 1.2f;
    float m_EatenSpeed = 3.0f;
    float m_FrightenedTimeRemaining = 0.0f;
    bool m_IgnoreFrightened = false;
    Direction m_CurrentDir = Direction::LEFT;
};

#endif
