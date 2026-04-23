#include "App.hpp"
#include "Core/Context.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/Time.hpp"

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
    ResetDeathSequence();

    m_CurrentState = State::UPDATE;
}

//讓 DYING 狀態下也能繼續畫當前畫面，但不做移動更新。死亡流程的細節在 App::Dying() 中實作。
void App::DrawGameplay() {
    m_Map.Draw();
    m_Scoreboard.Draw();
    m_Pacman.Draw();
    m_GhostManager.Draw();
}

//清掉死亡計時器、重設旗標、確保鬼先顯示、暫停 Pacman 動畫，然後切到 DYING。
void App::StartDeathSequence() {
    m_DeathSequenceTimer = 0.0f;
    m_HasHiddenGhosts = false;
    m_HasStartedDeathAnimation = false;
    m_GhostManager.SetVisible(true);
    m_Pacman.PauseAnimation();
    m_CurrentState = State::DYING;
}

//把死亡前置流程的計時器與旗標清回初始值。
void App::ResetDeathSequence() {
    m_DeathSequenceTimer = 0.0f;
    m_HasHiddenGhosts = false;
    m_HasStartedDeathAnimation = false;
    m_GhostManager.SetVisible(true);
}

void App::Update() {
    
    //Draw the map
    m_Map.Draw();
    //Draw the scoreboard
    m_Scoreboard.Draw();
    //Draw the pacman
    m_Pacman.Draw();
    //Draw the Ghost
    m_GhostManager.Update(m_Map, m_Pacman.GetPosition(), m_Pacman.GetDirection());
    m_GhostManager.Draw();
    
    

    points = m_Pacman.Update(m_Map);
    if (points > 0) {
        m_Scoreboard.AddScore(points); 
    }
            
    if (m_Map.IsLevelClear()){
        m_CurrentState = State::RESET;
    }

    else if (m_GhostManager.CheckCollision(m_Pacman.GetPosition())){
        StartDeathSequence();
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

void App::Dying() {
    m_DeathSequenceTimer += Util::Time::GetDeltaTimeMs() / 1000.0f;

    if (!m_HasHiddenGhosts && m_DeathSequenceTimer >= 0.5f) {
        m_GhostManager.SetVisible(false);
        m_HasHiddenGhosts = true;
    }

    if (!m_HasStartedDeathAnimation && m_DeathSequenceTimer >= 1.0f) {
        m_Pacman.StartDeathAnimation();
        m_HasStartedDeathAnimation = true;
    }

    DrawGameplay();

    if (m_HasStartedDeathAnimation && m_Pacman.IsDeathAnimationFinished()) {
        m_CurrentState = State::DEAD;
    }

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
        m_Scoreboard.ResetLives();
        m_Map.ResetData();
        m_Pacman.Reset();
        m_GhostManager.Reset();
        ResetDeathSequence();//避免下一命延續舊狀態。
        m_CurrentState = State::UPDATE;     
    }
}

void App::Dead() {
    if(m_Scoreboard.GetLives()==1){ 
        m_CurrentState = State::GAMEOVER;
    }
    DrawDead();
    m_GameText->Draw();

    //死亡不重置地圖
    if(Util::Input::IsKeyUp(Util::Keycode::TAB)){
        m_Scoreboard.MinusLives();
        m_Pacman.Reset();
        m_GhostManager.Reset();
        ResetDeathSequence();
        m_CurrentState = State::UPDATE;
    }
}

void App::Gameover() {
    DrawGameover();
    m_GameText->Draw();

    GameOverTimer+=0.016f;

    //1/60=0.016
    //差不多兩秒
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
