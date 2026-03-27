#include "App.hpp"
#include "Core/Context.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

void App::Start() {
    LOG_TRACE("Start");

    //Init Map
    m_Map.Start();

    //Init Pacman
    m_Pacman.Start();

    //Init Ghost
    m_GhostManager.Start(m_Map);

    //Init Scoreboard
    m_Scoreboard.Start();

    m_CurrentState = State::UPDATE;
}

void App::Update() {
    
    //Draw the map
    m_Map.Draw();
    //Draw the pacman
    m_Pacman.Draw();
    //Draw the Ghost
    m_GhostManager.Update(m_Map);
    m_GhostManager.Draw();
    //Draw the scoreboard
    m_Scoreboard.Draw();
    
    int points = m_Pacman.Update(m_Map);

    if (points > 0) {
        m_Scoreboard.AddScore(points); 
    }

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
