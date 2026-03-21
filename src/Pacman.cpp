#include "Pacman.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

void Pacman::Start() {
    m_Pacman = std::make_shared<Util::GameObject>();
    m_Pacman->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/Image/character/player.png"));
    
    m_Pacman->m_Transform.translation = {-288.0f, -288.0f};
    m_Pacman->SetZIndex(10);
}

void Pacman::Update(const Map& map) {
    auto pos = m_Pacman->m_Transform.translation;

    //Move Logic
    glm::vec2 nextPosY = pos;
    if (Util::Input::IsKeyPressed(Util::Keycode::W)) { nextPosY.y += m_Speed; }
    if (Util::Input::IsKeyPressed(Util::Keycode::S)) { nextPosY.y -= m_Speed; }

    if (!map.IsWall(pos.x, nextPosY.y)) {
        pos.y = nextPosY.y;
    }

    glm::vec2 nextPosX = pos;
    if (Util::Input::IsKeyPressed(Util::Keycode::A)) { nextPosX.x -= m_Speed; }
    if (Util::Input::IsKeyPressed(Util::Keycode::D)) { nextPosX.x += m_Speed; }

    if (!map.IsWall(nextPosX.x, pos.y)) {
        pos.x = nextPosX.x;
    }

    m_Pacman->m_Transform.translation = pos;
}

void Pacman::Draw() {
    m_Pacman->Draw();
}

/*
glm::vec2 Pacman::GetPosition() const {
    return m_Pacman->m_Transform.translation;
}
*/