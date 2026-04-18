#ifndef GHOST_HPP
#define GHOST_HPP

#include "pch.hpp"
#include "Util/GameObject.hpp"
#include "Map.hpp"
#include "Direction.hpp"
#include <memory>
#include <string>

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
    Ghost(const std::string& texturePath, glm::vec2 worldPos);
    virtual ~Ghost() = default;

    virtual void Update(const Map& map, glm::vec2 pacmanPos, Direction pacmanDir, GhostState state) = 0; 
    
    void Draw();
    glm::vec2 GetPosition() const;

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
    std::shared_ptr<Util::GameObject> m_GhostObj;
    float m_Speed = 3.0f;
};

#endif