#ifndef GHOST_HPP
#define GHOST_HPP

#include "pch.hpp"
#include "Util/GameObject.hpp"
#include "Map.hpp"
#include "Direction.hpp"
#include <memory>
#include <random>
#include <string>
#include <vector>

enum class GhostState {
        SCATTER,
        CHASE,
        FRIGHTENED
};

enum class HouseState {
        IN_HOUSE,
        EXITING,
        OUTSIDE
};


class Ghost {
public:
    enum class Direction {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        NONE
    };

    Ghost(const std::string& name,
          const std::string& texturePath,
          char displaySymbol,
          glm::vec2 worldPos);
    virtual ~Ghost() = default;

    virtual void Update(const Map& map, glm::vec2 pacmanPos, Direction pacmanDir, glm::vec2 blinkyPos, GhostState state) = 0; 
    
    void Draw();
    glm::vec2 GetPosition() const;
    const std::string& GetName() const;
    char GetDisplaySymbol() const;
    Direction GetCurrentDirection() const;

    void SetHouseRelease(float delaySeconds,
                         const std::vector<glm::vec2>& exitPath);

    void Reset();

    void SetIsActive(bool active);

    bool IsActive() const;

    void SetHouseState(HouseState s);
    HouseState GetHouseState() const;

protected:
    glm::vec2 m_SpawnPos;

    bool m_IsActive = false;

    HouseState m_HouseState = HouseState::IN_HOUSE;

protected:
    // 讓隨機移動的鬼怪直接重用共用流程，之後 Week 11 可以再換成更進階 AI。
    void UpdateRandomMovement(const Map& map);
    bool UpdateHouseRelease();

    bool IsColliding(const Map& map, glm::vec2 pos) const;
    bool IsAtGridCenter(const Map& map, glm::vec2 pos) const;

    glm::vec2 GetMoveOffset(Direction direction) const;
    glm::vec2 GetTileOffset(Direction direction) const;

    std::vector<Direction> GetAvailableDirections(
        const Map& map,
        glm::vec2 centerPos,
        bool allowReverse) const;

    Direction PickRandomDirection(const std::vector<Direction>& directions);
    Direction GetOppositeDirection(Direction direction) const;

    void SetDirection(Direction direction);
    void SetPosition(glm::vec2 pos);

    float GetSpeed() const;
    float GetCollisionRadius() const;

private:
    std::shared_ptr<Util::GameObject> m_GhostObj;
    std::string m_Name;
    char m_DisplaySymbol = '?';
    Direction m_CurrentDir = Direction::LEFT;
    // 保留原本較接近 Pac-Man 的移動節奏。
    float m_Speed = 3.0f;
    float m_CollisionRadius = 14.0f;
    std::mt19937 m_Rng;

    float m_ReleaseDelay = 0.0f;
    float m_ReleaseTimer = 0.0f;
    bool m_IsReleased = true;
    std::vector<glm::vec2> m_HouseExitPath;
    std::size_t m_CurrentExitTarget = 0;
};

#endif
