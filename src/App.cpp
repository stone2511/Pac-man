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
    }

    else if (m_GhostManager.CheckCollision(m_Pacman.GetPosition())){
        m_CurrentState = State::DEAD;
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

    DrawVictory();
    m_GameText->Draw();

    if(Util::Input::IsKeyUp(Util::Keycode::TAB)){
        m_Scoreboard.NextLevel();
        m_Map.ResetData();
        m_Pacman.Reset();
        m_GhostManager.Reset();
        m_CurrentState = State::UPDATE;     
    }
}

void App::Dead() {
    if(m_Scoreboard.GetLives()==1){ 
        m_CurrentState = State::GAMEOVER;
    }
    DrawDead();
    m_GameText->Draw();

    if(Util::Input::IsKeyUp(Util::Keycode::TAB)){
        m_Scoreboard.MinusLives();
        m_Pacman.Reset();
        m_GhostManager.Reset();
        m_CurrentState = State::UPDATE;
    }
}

void App::Gameover() {
    DrawGameover();
    m_GameText->Draw();

    GameOverTimer+=0.016f;

    if(GameOverTimer > 2.0f){
        m_CurrentState = State::END;
    }
}

void App::DrawVictory() {
    m_GameText = std::make_shared<Util::GameObject>();
    m_GameText->SetDrawable(std::make_shared<Util::Text>(
        RESOURCE_DIR"/font/inkfree.ttf", 
        50, 
        "Victory(Press Tab to NextLevel)", 
        Util::Color::FromName(Util::Colors::YELLOW)
    ));
    m_GameText->m_Transform.translation = {0.0f, 0.0f};
}

void App::DrawDead() {
    m_GameText = std::make_shared<Util::GameObject>();
    m_GameText->SetDrawable(std::make_shared<Util::Text>(
        RESOURCE_DIR"/font/inkfree.ttf", 
        50, 
        "You dead. (Press Tab to Continue)", 
        Util::Color::FromName(Util::Colors::YELLOW)
    ));
    m_GameText->m_Transform.translation = {0.0f, 0.0f};
}

void App::DrawGameover(){
     m_GameText = std::make_shared<Util::GameObject>();
    m_GameText->SetDrawable(std::make_shared<Util::Text>(
        RESOURCE_DIR"/font/inkfree.ttf", 
        50, 
        "Game Over!!!", 
        Util::Color::FromName(Util::Colors::YELLOW)
    ));
    m_GameText->m_Transform.translation = {0.0f, 0.0f};
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
