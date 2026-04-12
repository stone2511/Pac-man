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
    m_GhostManager.Update(m_Map, m_Pacman.GetPosition());
    m_GhostManager.Draw();
    //Draw the scoreboard
    m_Scoreboard.Draw();
    

    points = m_Pacman.Update(m_Map);
    if (points > 0) {
        m_Scoreboard.AddScore(points); 
    }
            
    if (m_Map.IsLevelClear()){
        m_CurrentState = State::RESET;
        //m_LevelClearTime = 0.0f;
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

void App::Reset() {
    //m_LevelClearTime += 0.016f;
    DrawVictory();
            
    if(Util::Input::IsKeyUp(Util::Keycode::TAB)){
        m_Scoreboard.NextLevel();
        m_Map.ResetData();
        m_Pacman.Reset();
        m_GhostManager.Reset();
        m_CurrentState = State::UPDATE;     
    }
}

void App::DrawVictory(){
    m_GameText = std::make_shared<Util::GameObject>();
    m_GameText->SetDrawable(std::make_shared<Util::Text>(
        RESOURCE_DIR"/font/inkfree.ttf", 
        50, 
        "Victory(Press Tab to NextLevel)", 
        Util::Color::FromName(Util::Colors::YELLOW)
    ));
    m_GameText->m_Transform.translation = {0.0f, 0.0f};
    m_GameText->Draw();
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
