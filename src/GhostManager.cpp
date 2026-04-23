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
    m_ReleaseTimer += 0.016f;

    glm::vec2 blinkyPos = m_Ghosts[0]->GetPosition();

    for(size_t i=0 ; i<m_Ghosts.size() ; ++i) {
        if(!m_Ghosts[i]->IsActive() && m_ReleaseTimer >= m_Release[i]){
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

    //7秒Scatter狀態20秒Chase狀態
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

//一次設定所有鬼的顯示狀態，死亡流程在 0.5s 到時直接呼叫它，把全部鬼隱藏。
void GhostManager::SetVisible(bool visible) {
    for (auto& ghost : m_Ghosts) {
        ghost->SetVisible(visible);
    }
}

void GhostManager::Reset(){
    m_CurrentState = GhostState::SCATTER;
    m_StateTimer = 0.0f;
    m_ReleaseTimer = 0.0f;
    SetVisible(true);//避免上一條命死掉後鬼還保持隱藏。

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
        //檢查 Pacman 是否碰到任何鬼。只會檢查「已啟用而且可見」的鬼，所以鬼消失後不會再重複判定碰撞。
        if (!ghost->IsActive() || !ghost->IsVisible()) {
            continue;
        }

        float distance = glm::distance(pacmanPos, ghost->GetPosition());

        if(distance < death_radius){
            return true;
        }
    }
    
    return false;
}
