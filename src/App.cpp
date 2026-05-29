#include "App.hpp"
#include "Core/Context.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/Time.hpp"
#include <cmath>

LevelConfig App::GetLevelConfig(int level){
    LevelConfig config;
    config.mapIndex = (level-1) % 5;
    int l;
    if(level>=10) l = 1;
    else if(level>=20) l = 2;
    else l = 0;
    config.ghostSpeed = 2.0f + (l*0.5f);
    if(config.ghostSpeed >= 4.0f) config.ghostSpeed = 4.0f;

    config.frightenedDuration = 8.0f - (level * 0.25f);
    if(config.frightenedDuration <= 3.0f) config.frightenedDuration = 3.0f;

    float cycle = 10.0f - (level * 0.5f);
    if(cycle <= 0.0f) cycle = 0.0f;
    config.release = {0.0f, cycle*0.3f, cycle*0.6f, cycle};

    return config;
}

void App::Start() {
    LOG_TRACE("Start");

    //Init Map
    m_Map.Start(0);

    //Init Pacman
    m_Pacman.Start();

    //Init Ghost
    m_GhostManager.Start(m_Map);

    //Init Scoreboard
    m_Scoreboard.Start();
    ResetDeathSequence();
    ResetFruit();

    m_CurrentState = State::UPDATE;
}

//讓 DYING 狀態下也能繼續畫當前畫面，但不做移動更新。死亡流程的細節在 App::Dying() 中實作。
void App::DrawGameplay() {
    m_Map.Draw();
    DrawFruit();
    m_Scoreboard.Draw();
    m_Pacman.Draw();
    m_GhostManager.Draw();
}

bool App::HandleLevelShortcut() {
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_1)) {
        LoadLevel(1);
        return true;
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_2)) {
        LoadLevel(2);
        return true;
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_3)) {
        LoadLevel(3);
        return true;
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_4)) {
        LoadLevel(4);
        return true;
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_5)) {
        LoadLevel(5);
        return true;
    }

    return false;
}

void App::LoadLevel(int newLevel) {
    level = newLevel;
    m_Scoreboard.SetLevel(level);
    m_Map.Start(level);
    m_Pacman.Reset();
    m_GhostManager.Start(m_Map);
    ResetDeathSequence();
    ResetFruit();
    m_CurrentState = State::UPDATE;
}

//清掉死亡計時器、重設旗標、確保鬼先顯示、暫停 Pacman 動畫，然後切到 DYING。
void App::StartDeathSequence() {
    m_DeathSequenceTimer = 0.0f;
    m_HasHiddenGhosts = false;
    m_HasStartedDeathAnimation = false;
    m_GhostManager.SetVisible(true);
    m_IsFruitVisible = false;
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

void App::ResetFruit() {
    m_BeansEatenThisLevel = 0;
    m_HasFruitSpawnedThisLevel = false;
    m_IsFruitVisible = false;
    m_FruitTimer = 0.0f;
    m_FruitScoreTextTimer = 0.0f;
    m_FruitScoreText = nullptr;
    LoadFruitForLevel();
}

void App::LoadFruitForLevel() {
    m_CurrentFruitScore = GetFruitScoreForLevel(level);
    m_Fruit = std::make_shared<Util::GameObject>();
    m_Fruit->SetDrawable(std::make_shared<Util::Image>(GetFruitImageForLevel(level)));
    m_Fruit->m_Transform.translation = m_Map.GridToWorld(10.0f, 11.0f);
    m_Fruit->SetZIndex(6);
}

void App::SpawnFruit() {
    if (m_Fruit == nullptr) {
        LoadFruitForLevel();
    }

    m_IsFruitVisible = true;
    m_HasFruitSpawnedThisLevel = true;
    m_FruitTimer = 0.0f;
}

void App::UpdateFruit() {
    if (!m_IsFruitVisible || m_Fruit == nullptr) {
        return;
    }

    m_FruitTimer += Util::Time::GetDeltaTimeMs() / 1000.0f;
    if (m_FruitTimer >= m_FruitVisibleDuration) {
        m_IsFruitVisible = false;
        return;
    }

    const glm::vec2 fruitPos = m_Fruit->m_Transform.translation;
    const glm::vec2 pacmanPos = m_Pacman.GetPosition();
    constexpr float eatRadius = 18.0f;
    if (std::abs(pacmanPos.x - fruitPos.x) < eatRadius &&
        std::abs(pacmanPos.y - fruitPos.y) < eatRadius) {
        m_Scoreboard.AddScore(m_CurrentFruitScore);
        ShowFruitScore(fruitPos);
        m_IsFruitVisible = false;
    }
}

void App::ShowFruitScore(glm::vec2 position) {
    m_FruitScoreText = std::make_shared<Util::GameObject>();
    m_FruitScoreText->SetDrawable(std::make_shared<Util::Text>(
        RESOURCE_DIR"/font/inkfree.ttf",
        12,
        std::to_string(m_CurrentFruitScore),
        Util::Color::FromName(Util::Colors::WHITE)
    ));
    m_FruitScoreText->m_Transform.translation = position;
    m_FruitScoreText->SetZIndex(20);
    m_FruitScoreTextTimer = 0.0f;
}

void App::UpdateFruitScoreText() {
    if (m_FruitScoreText == nullptr) {
        return;
    }

    m_FruitScoreTextTimer += Util::Time::GetDeltaTimeMs() / 1000.0f;
    if (m_FruitScoreTextTimer >= m_FruitScoreTextDuration) {
        m_FruitScoreText = nullptr;
        m_FruitScoreTextTimer = 0.0f;
    }
}

void App::DrawFruit() {
    if (m_IsFruitVisible && m_Fruit != nullptr) {
        m_Fruit->Draw();
    }
    if (m_FruitScoreText != nullptr) {
        m_FruitScoreText->Draw();
    }
}

int App::GetFruitScoreForLevel(int level) const {
    if (level == 1) {
        return 100;
    }
    if (level == 2) {
        return 300;
    }
    if (level <= 4) {
        return 500;
    }
    if (level <= 6) {
        return 700;
    }
    if (level <= 8) {
        return 1000;
    }
    if (level <= 10) {
        return 2000;
    }
    if (level <= 12) {
        return 3000;
    }
    return 5000;
}

std::string App::GetFruitImageForLevel(int level) const {
    if (level == 1) {
        return RESOURCE_DIR"/Image/fruit/Cherry.png";
    }
    if (level == 2) {
        return RESOURCE_DIR"/Image/fruit/Strawberry.png";
    }
    if (level <= 4) {
        return RESOURCE_DIR"/Image/fruit/Orange.png";
    }
    if (level <= 6) {
        return RESOURCE_DIR"/Image/fruit/Apple.png";
    }
    if (level <= 8) {
        return RESOURCE_DIR"/Image/fruit/Melon.png";
    }
    if (level <= 10) {
        return RESOURCE_DIR"/Image/fruit/Galaxian.png";
    }
    if (level <= 12) {
        return RESOURCE_DIR"/Image/fruit/Bell.png";
    }
    return RESOURCE_DIR"/Image/fruit/Key.png";
}

void App::Update() {
    if (HandleLevelShortcut()) {
        return;
    }
    
    m_Map.Draw();
    DrawFruit();
    m_Scoreboard.Draw();
    m_Pacman.Draw();
    //Draw the Ghost
    m_GhostManager.Update(m_Map, m_Pacman.GetPosition(), m_Pacman.GetDirection(),level);
    m_GhostManager.Draw();

    const BeanEatResult eatResult = m_Pacman.Update(m_Map);
    if (eatResult.score > 0) {
        m_Scoreboard.AddScore(eatResult.score); 
    }
    m_BeansEatenThisLevel += eatResult.beansEaten;

    if (!m_HasFruitSpawnedThisLevel &&
        m_BeansEatenThisLevel >= m_FruitSpawnBeanCount) {
        SpawnFruit();
    }

    UpdateFruit();
    UpdateFruitScoreText();

    if (eatResult.atePowerPellet) {
        m_GhostManager.TriggerPowerMode();
    }
            
    if (Util::Input::IsKeyUp(Util::Keycode::E)){
        level++;
        m_CurrentState = State::RESET;
    }



    if (m_Map.IsLevelClear()){
        level++;
        m_CurrentState = State::RESET;
    }

    else {
        const GhostCollisionResult collisionResult =
            m_GhostManager.ResolveCollision(m_Pacman.GetPosition());

        if (collisionResult == GhostCollisionResult::PACMAN_DIED) {
            StartDeathSequence();
        } else if (collisionResult == GhostCollisionResult::GHOST_EATEN) {
            m_Scoreboard.AddScore(m_GhostManager.GetGhostEatScore());
        }
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
    if (HandleLevelShortcut()) {
        return;
    }

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
    if (HandleLevelShortcut()) {
        return;
    }

    DrawVictory();
    m_GameText->Draw();
    LevelConfig config = GetLevelConfig(level);

    if(Util::Input::IsKeyUp(Util::Keycode::TAB)){
        
        m_Scoreboard.NextLevel();
        m_Scoreboard.ResetLives();
        m_Map.Start(config.mapIndex);
        m_Pacman.Reset();
        m_GhostManager.Reset(config);
        ResetFruit();
        ResetDeathSequence();//避免下一命延續舊狀態。
        m_CurrentState = State::UPDATE;     
    }
}

void App::Dead() {
    if (HandleLevelShortcut()) {
        return;
    }

    if(m_Scoreboard.GetLives()==1){ 
        m_CurrentState = State::GAMEOVER;
    }
    DrawDead();
    m_GameText->Draw();

    //死亡不重置地圖
    if(Util::Input::IsKeyUp(Util::Keycode::TAB)){
        LevelConfig config = GetLevelConfig(level);
        m_Scoreboard.MinusLives();
        m_Pacman.Reset();
        m_GhostManager.Reset(config);
        ResetDeathSequence();
        m_CurrentState = State::UPDATE;
    }
}

void App::Gameover() {
    if (HandleLevelShortcut()) {
        GameOverTimer = 0.0f;
        return;
    }

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
        25, 
        "Victory(Press Tab to NextLevel)", 
        Util::Color::FromName(Util::Colors::YELLOW)
    ));
    m_GameText->m_Transform.translation = {0.0f, 0.0f};
}

void App::DrawDead() {
    m_GameText = std::make_shared<Util::GameObject>();
    m_GameText->SetDrawable(std::make_shared<Util::Text>(
        RESOURCE_DIR"/font/inkfree.ttf", 
        25, 
        "You dead. (Press Tab to Continue)", 
        Util::Color::FromName(Util::Colors::YELLOW)
    ));
    m_GameText->m_Transform.translation = {0.0f, 0.0f};
}

void App::DrawGameover(){
    m_GameText = std::make_shared<Util::GameObject>();
    m_GameText->SetDrawable(std::make_shared<Util::Text>(
        RESOURCE_DIR"/font/inkfree.ttf", 
        25, 
        "Game Over!!!", 
        Util::Color::FromName(Util::Colors::YELLOW)
    ));
    m_GameText->m_Transform.translation = {0.0f, 0.0f};
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
