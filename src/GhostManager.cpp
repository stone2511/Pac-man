#include "GhostManager.hpp"
#include "Ghost_Blinky.hpp"
#include "Ghost_Pinky.hpp"
#include "Ghost_Inky.hpp"
#include "Ghost_Clyde.hpp"

void GhostManager::Start(const Map& map) {
    m_Ghosts.clear();
    m_Ghosts.push_back(std::make_shared<Ghost_Blinky>(map.GridToWorld(10.0f, 7.0f)));
    m_Ghosts.push_back(std::make_shared<Ghost_Pinky>(map.GridToWorld(8.9f, 9.0f)));
    m_Ghosts.push_back(std::make_shared<Ghost_Inky>(map.GridToWorld(10.0f, 9.0f)));
    m_Ghosts.push_back(std::make_shared<Ghost_Clyde>(map.GridToWorld(11.1f, 9.0f)));
}

void GhostManager::Update(const Map& map) {
    for (auto& ghost : m_Ghosts) {
        ghost->Update(map); 
    }
}

void GhostManager::Draw() {
    for (auto& ghost : m_Ghosts) {
        ghost->Draw();
    }
}
