#ifndef GHOST_HPP
#define GHOST_HPP

#include "pch.hpp"
#include "Util/GameObject.hpp"
#include "Map.hpp"
#include <memory>
#include <string>

class Ghost {
public:
    Ghost(const std::string& texturePath, glm::vec2 worldPos);
    virtual ~Ghost() = default;

    virtual void Update(const Map& map) = 0; 
    
    void Draw();
    glm::vec2 GetPosition() const;

protected:
    std::shared_ptr<Util::GameObject> m_GhostObj;
    float m_Speed = 4.0f;
};

#endif