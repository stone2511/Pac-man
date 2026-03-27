#include "Ghost.hpp"
#include "Util/Image.hpp"

Ghost::Ghost(const std::string& texturePath, glm::vec2 worldPos) {
    m_GhostObj = std::make_shared<Util::GameObject>();
    m_GhostObj->SetDrawable(std::make_shared<Util::Image>(texturePath));
    m_GhostObj->m_Transform.translation = worldPos;
    m_GhostObj->SetZIndex(5);
}

void Ghost::Draw() {
    m_GhostObj->Draw();
}

glm::vec2 Ghost::GetPosition() const {
    return m_GhostObj->m_Transform.translation;
}