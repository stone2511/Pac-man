#include "GhostManager.hpp"
#include "Ghost_Blinky.hpp"
#include "Ghost_Pinky.hpp"
#include "Ghost_Inky.hpp"
#include "Ghost_Clyde.hpp"
#include "Util/Time.hpp"

void GhostManager::Start(const Map& map) {
    m_Ghosts.clear();
    auto blinky = std::make_shared<Ghost_Blinky>(map.GridToWorld(10.0f, 7.0f));
    blinky->SetHomePosition(map.GridToWorld(10.0f, 9.0f));
    m_Ghosts.push_back(blinky);
    auto pinky = std::make_shared<Ghost_Pinky>(map.GridToWorld(10.0f, 9.0f));
    pinky->SetHomePosition(map.GridToWorld(10.0f, 9.0f));
    m_Ghosts.push_back(pinky);
    auto inky = std::make_shared<Ghost_Inky>(map.GridToWorld(8.9f, 9.0f));
    inky->SetHomePosition(map.GridToWorld(9.0f, 9.0f));
    m_Ghosts.push_back(inky);
    auto clyde = std::make_shared<Ghost_Clyde>(map.GridToWorld(11.1f, 9.0f));
    clyde->SetHomePosition(map.GridToWorld(11.0f, 9.0f));
    m_Ghosts.push_back(clyde);
    Reset();
}

void GhostManager::Update(const Map& map, glm::vec2 pacmanPos, Direction pacmanDir) {
    const float deltaTime = Util::Time::GetDeltaTimeMs() / 1000.0f;
    m_ReleaseTimer += deltaTime;

    if (m_CurrentState == GhostState::FRIGHTENED) {
        m_FrightenedTimer -= deltaTime;
        if (m_FrightenedTimer <= 0.0f) {
            m_FrightenedTimer = 0.0f;
            m_CurrentState = m_NormalState;
        }
    } else {
        m_StateTimer += deltaTime;

        if (m_NormalState == GhostState::SCATTER && m_StateTimer >= 7.0f) {
            m_NormalState = GhostState::CHASE;
            m_StateTimer = 0.0f;
        } else if (m_NormalState == GhostState::CHASE && m_StateTimer >= 20.0f) {
            m_NormalState = GhostState::SCATTER;
            m_StateTimer = 0.0f;
        }

        m_CurrentState = m_NormalState;
    }

    glm::vec2 blinkyPos = m_Ghosts[0]->GetPosition();

    for(size_t i=0 ; i<m_Ghosts.size() ; ++i) {
        if(!m_Ghosts[i]->IsActive() && m_ReleaseTimer >= m_Release[i]){
            m_Ghosts[i]->SetIsActive(true);
            m_Ghosts[i]->SetHouseState(HouseState::EXITING);
        }

        if(m_Ghosts[i]->IsActive()){
            GhostState ghostState = m_CurrentState;
            if (m_Ghosts[i]->IsReturningToHouse()) {
                ghostState = GhostState::EATEN;
            }

            if (ghostState == GhostState::FRIGHTENED && m_Ghosts[i]->IgnoresFrightened()) {
                ghostState = m_NormalState;
            }

            m_Ghosts[i]->SetFrightenedTimeRemaining(
                ghostState == GhostState::FRIGHTENED ? m_FrightenedTimer : 0.0f
            );
            m_Ghosts[i]->Update(map, pacmanPos, pacmanDir, blinkyPos, ghostState);
        }
        else{
            //todo
        }
    }
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
    if (m_Ghosts.empty()) {
        return;
    }

    m_NormalState = GhostState::SCATTER;
    m_CurrentState = GhostState::SCATTER;
    m_StateTimer = 0.0f;
    m_ReleaseTimer = 0.0f;
    m_FrightenedTimer = 0.0f;
    m_GhostEatChain = 0;
    SetVisible(true);//避免上一條命死掉後鬼還保持隱藏。

    for(size_t i=0 ; i<m_Ghosts.size() ; ++i){
        m_Ghosts[i]->Reset();
        m_Ghosts[i]->SetIsActive(i==0);
        m_Ghosts[i]->SetHouseState(i == 0 ? HouseState::EXITING : HouseState::IN_HOUSE);
    }
}

void GhostManager::TriggerPowerMode() {
    m_CurrentState = GhostState::FRIGHTENED;
    m_FrightenedTimer = m_FrightenedDuration;
    m_GhostEatChain = 0;

    for (auto& ghost : m_Ghosts) {
        if (!ghost->IsReturningToHouse()) {
            ghost->ResetFrightenedImmunity();
        }
        if (ghost->IsActive() && ghost->GetHouseState() == HouseState::OUTSIDE) {
            ghost->ReverseDirection();
        }
    }
}

int GhostManager::GetGhostEatScore() const {
    if (m_GhostEatChain <= 0) {
        return 200;
    }

    return 200 * (1 << (m_GhostEatChain - 1));
}

GhostCollisionResult GhostManager::ResolveCollision(glm::vec2 pacmanPos) {
    float death_radius = 18.0f;

    for (auto& ghost : m_Ghosts){
        //檢查 Pacman 是否碰到任何鬼。只會檢查「已啟用而且可見」的鬼，所以鬼消失後不會再重複判定碰撞。
        if (!ghost->IsActive() || !ghost->IsVisible() || ghost->IsReturningToHouse()) {
            continue;
        }

        float distance = glm::distance(pacmanPos, ghost->GetPosition());

        if(distance < death_radius){
            if (m_CurrentState == GhostState::FRIGHTENED && !ghost->IgnoresFrightened()) {
                ghost->BecomeEaten();
                ++m_GhostEatChain;
                return GhostCollisionResult::GHOST_EATEN;
            }
            return GhostCollisionResult::PACMAN_DIED;
        }
    }
    
    return GhostCollisionResult::NONE;
}
