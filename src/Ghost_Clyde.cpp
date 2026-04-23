#include "Ghost_Clyde.hpp"


Ghost_Clyde::Ghost_Clyde(glm::vec2 worldPos) 
    : Ghost(RESOURCE_DIR"/Image/ghost/clyde0.png", worldPos) {
}

void Ghost_Clyde::Update(const Map& map, glm::vec2 pacmanPos, Direction pacmanDir, glm::vec2 blinkyPos, GhostState state) {
    auto pos = m_GhostObj->m_Transform.translation;

    //出門邏輯
    if(m_HouseState == HouseState::EXITING){
        glm::vec2 doorPos = map.GridToWorld(10,7);
        float exitSpeed = 2.0f;

        if(std::abs(pos.x - doorPos.x) > exitSpeed){
            pos.x += (pos.x < doorPos.x) ? exitSpeed : -exitSpeed;
        }
        else{
            pos.x = doorPos.x;
            if(std::abs(pos.y - doorPos.y) > exitSpeed){
                pos.y += (pos.y < doorPos.y) ? exitSpeed : -exitSpeed;
            }
            else{
                pos.y = doorPos.y;
                m_HouseState = HouseState::OUTSIDE;
                m_CurrentDir = Direction::LEFT;
            }
        }

        m_GhostObj->m_Transform.translation = pos;
        return;
    }

    glm::vec2 gridCenter = map.GetClosestGridCenter(pos.x, pos.y);

    glm::vec2 targetPos;
    
    if (state == GhostState::CHASE) {
        float distance = glm::distance(pos, pacmanPos);
        float D_distance = 8.0f * 32.0f;

        if(distance > D_distance){
            targetPos = pacmanPos;
        }
        else{
            targetPos = glm::vec2(-999.0f, -999.0f);
        }
    } 
    else if (state == GhostState::SCATTER) {
        targetPos = glm::vec2(-999.0f, -999.0f); 
    }

    //1. Decision time

    if(std::abs(pos.x - gridCenter.x)< m_Speed && std::abs(pos.y - gridCenter.y) < m_Speed) {
        
        pos = gridCenter;

        float distUp = std::numeric_limits<float>::max();
        float distDown = std::numeric_limits<float>::max();
        float distLeft = std::numeric_limits<float>::max();
        float distRight = std::numeric_limits<float>::max();

        if (!map.IsWall(pos.x, pos.y + 32.0f) && m_CurrentDir != Direction::DOWN) {
            distUp = std::pow(pos.x - targetPos.x, 2) + std::pow((pos.y + 32.0f) - targetPos.y, 2);
        }

        if (!map.IsDoor(pos.x, pos.y - 32.0f) && !map.IsWall(pos.x, pos.y - 32.0f) && m_CurrentDir != Direction::UP) {
            distDown = std::pow(pos.x - targetPos.x, 2) + std::pow((pos.y - 32.0f) - targetPos.y, 2);
        }

        if (!map.IsWall(pos.x - 32.0f, pos.y) && m_CurrentDir != Direction::RIGHT) {
            distLeft = std::pow((pos.x - 32.0f) - targetPos.x, 2) + std::pow(pos.y - targetPos.y, 2);
        }

        if (!map.IsWall(pos.x + 32.0f, pos.y) && m_CurrentDir != Direction::LEFT) {
            distRight = std::pow((pos.x + 32.0f) - targetPos.x, 2) + std::pow(pos.y - targetPos.y, 2);
        }

        float minDist = std::min({distUp, distLeft, distDown, distRight});

        if (minDist == distUp) { m_CurrentDir = Direction::UP; }
        else if (minDist == distLeft) { m_CurrentDir = Direction::LEFT; }
        else if (minDist == distDown) { m_CurrentDir = Direction::DOWN; }
        else if (minDist == distRight) { m_CurrentDir = Direction::RIGHT; }

    }

    //2. Action time


    switch (m_CurrentDir) {
        case Direction::UP:    pos.y += m_Speed; break;
        case Direction::DOWN:  pos.y -= m_Speed; break;
        case Direction::LEFT:  pos.x -= m_Speed; break;
        case Direction::RIGHT: pos.x += m_Speed; break;
        default: break;
    }


    map.TryWrapTunnel(pos, 16.0f);

    m_GhostObj->m_Transform.translation = pos;
}
