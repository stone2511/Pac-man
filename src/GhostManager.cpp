#include "GhostManager.hpp"
#include "Ghost_Blinky.hpp"
#include "Ghost_Pinky.hpp"
#include "Ghost_Inky.hpp"
#include "Ghost_Clyde.hpp"

void GhostManager::Start(const Map& map) {
    m_Ghosts.clear();
    m_Ghosts.push_back(std::make_shared<Ghost_Blinky>(map.GridToWorld(10.0f, 7.0f)));
    m_Ghosts.push_back(std::make_shared<Ghost_Pinky>(map.GridToWorld(10.0f, 9.0f)));
    m_Ghosts.push_back(std::make_shared<Ghost_Inky>(map.GridToWorld(8.9f, 9.0f)));
    m_Ghosts.push_back(std::make_shared<Ghost_Clyde>(map.GridToWorld(11.1f, 9.0f)));
}

void GhostManager::Update(const Map& map, glm::vec2 pacmanPos, Direction pacmanDir) {
    m_StateTimer += 0.016f; 
    m_RealseTimer += 0.016f;

    glm::vec2 blinkyPos = m_Ghosts[0]->GetPosition();

    for(size_t i=0 ; i<m_Ghosts.size() ; ++i) {
        if(!m_Ghosts[i]->IsActive() && m_RealseTimer >= m_Realse[i]){
            m_Ghosts[i]->SetIsActive(true);
            m_Ghosts[i]->SetHouseState(HouseState::EXITING);
        }

        if(m_Ghosts[i]->IsActive()){
            m_Ghosts[i]->Update(map, pacmanPos, pacmanDir, blinkyPos, m_CurrentState);
        }
        else{
            //todo
        }
    }

    if (m_CurrentState == GhostState::SCATTER && m_StateTimer >= 7.0f) {
        m_CurrentState = GhostState::CHASE;
        m_StateTimer = 0.0f;
    } 
    else if (m_CurrentState == GhostState::CHASE && m_StateTimer >= 20.0f) {
        m_CurrentState = GhostState::SCATTER;
        m_StateTimer = 0.0f;
    }
/*
    for (auto& ghost : m_Ghosts) {
        ghost->Update(map, pacmanPos, m_CurrentState); 
    }
*/
    
}

void GhostManager::Draw() {
    for (auto& ghost : m_Ghosts) {
        ghost->Draw();
    }
}

void GhostManager::Reset(){
    m_CurrentState = GhostState::SCATTER;
    m_StateTimer = 0.0f;
    m_RealseTimer = 0.0f;

    for(size_t i=0 ; i<m_Ghosts.size() ; ++i){
        m_Ghosts[i]->Reset();
        m_Ghosts[i]->SetIsActive(i==0);
    }
/*
    for (auto& ghost : m_Ghosts) {
        ghost->Reset();
    }
*/ 
    
}

bool GhostManager::CheckCollision(glm::vec2 pacmanPos) const {
    float death_radius = 18.0f;

    for (const auto& ghost : m_Ghosts){
        float distance = glm::distance(pacmanPos, ghost->GetPosition());

        if(distance < death_radius){
            return true;
        }
    }
    
    return false;
}

