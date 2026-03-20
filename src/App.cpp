#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

void App::Start() {
    LOG_TRACE("Start");
    m_CurrentState = State::UPDATE;

    m_Position = {336.0f, 432.0f};
}

void App::Update() {
    
    float speed = 5.0f;

    if (Util::Input::IsKeyPressed(Util::Keycode::W)) {
        m_Position.y -= speed; 
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::S)) {
        m_Position.y += speed; 
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::A)) {
        m_Position.x -= speed; 
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::D)) {
        m_Position.x += speed; 
    }
    
    ImGui::GetBackgroundDrawList()->AddCircleFilled(
        ImVec2(m_Position.x, m_Position.y), 
        20.0f, 
        IM_COL32(255, 255, 0, 255) // 黃色 (Red=255, Green=255, Blue=0, Alpha=255)
    );
    
    /*
     * Do not touch the code below as they serve the purpose for
     * closing the window.
     */
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
