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

void Pacman::Update() {
    auto pos = m_Pacman->m_Transform.translation;

    //Move Logic
    if (Util::Input::IsKeyPressed(Util::Keycode::W)) { pos.y += m_Speed; }
    if (Util::Input::IsKeyPressed(Util::Keycode::S)) { pos.y -= m_Speed; }
    if (Util::Input::IsKeyPressed(Util::Keycode::A)) { pos.x -= m_Speed; }
    if (Util::Input::IsKeyPressed(Util::Keycode::D)) { pos.x += m_Speed; }

    m_Pacman->m_Transform.translation = pos;
}

void Pacman::Draw() {
    m_Pacman->Draw();
}

glm::vec2 Pacman::GetPosition() const {
    return m_Pacman->m_Transform.translation;
}