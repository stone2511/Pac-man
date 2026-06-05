#include "App.hpp"
#include "Core/Context.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/Color.hpp"
#include "Util/Text.hpp"
#include "Util/Time.hpp"

LevelConfig App::GetLevelConfig(int level){
    LevelConfig config;
    config.mapIndex = (level-1) % 5;

    int difficultyTier = level/5;
    /*
    if(level >= 5){
        difficultyTier = 1;
    }
    if(level >= 10){
        difficultyTier = 2;
    }
    if(level >= 15){
        difficultyTier = 3;
    }
    if(level >= 20){
        difficultyTier = 4;
    }
    if(level >= 25){
        difficultyTier = 5;
    }
    */
    
    config.ghostSpeed = 2.0f + (difficultyTier * 0.5f);
    if(config.ghostSpeed >= 4.0f) config.ghostSpeed = 4.0f;

    config.frightenedDuration = 8.0f - (difficultyTier * 1.0f);
    if(config.frightenedDuration <= 3.0f) config.frightenedDuration = 3.0f;

    float cycle = 10.0f - (difficultyTier * 2.0f);
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
    m_Fruit.Draw();
    m_Scoreboard.Draw();
    m_Pacman.Draw();
    m_GhostManager.Draw();
    if (m_GhostEatScoreText != nullptr) {
        m_GhostEatScoreText->Draw();
    }
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

bool App::HandleExitInput() {
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
        return true;
    }

    return false;
}

void App::LoadLevel(int newLevel) {
    level = newLevel;
    const LevelConfig config = GetLevelConfig(level);
    m_Scoreboard.SetLevel(level);
    m_Map.Start(config.mapIndex);
    m_Pacman.Reset();
    m_GhostManager.Start(m_Map);
    m_GhostManager.Reset(config);
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
    m_Fruit.Hide();
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
    m_Fruit.Reset(level, m_Map);
    m_GhostEatPauseTimer = 0.0f;
    m_GhostEatScoreTextTimer = 0.0f;
    m_GhostEatScoreText = nullptr;
}

void App::ShowGhostEatScore(glm::vec2 position, int score) {
    m_GhostEatScoreText = std::make_shared<Util::GameObject>();
    m_GhostEatScoreText->SetDrawable(std::make_shared<Util::Text>(
        RESOURCE_DIR"/font/inkfree.ttf",
        12,
        std::to_string(score),
        Util::Color::FromName(Util::Colors::WHITE)
    ));
    m_GhostEatScoreText->m_Transform.translation = position;
    m_GhostEatScoreText->SetZIndex(20);
    m_GhostEatScoreTextTimer = 0.0f;
}

void App::UpdateGhostEatScoreText() {
    if (m_GhostEatScoreText == nullptr) {
        return;
    }

    m_GhostEatScoreTextTimer += Util::Time::GetDeltaTimeMs() / 1000.0f;
    if (m_GhostEatScoreTextTimer >= m_GhostEatScoreTextDuration) {
        m_GhostEatScoreText = nullptr;
        m_GhostEatScoreTextTimer = 0.0f;
    }
}

void App::Update() {
    if (HandleLevelShortcut()) {
        return;
    }
    
    m_Map.Draw();
    m_Fruit.Draw();
    m_Scoreboard.Draw();
    m_Pacman.Draw();
    //Draw the Ghost
    m_GhostManager.Update(m_Map, m_Pacman.GetPosition(), m_Pacman.GetDirection(),level);
    m_GhostManager.Draw();

    const BeanEatResult eatResult = m_Pacman.Update(m_Map);
    if (eatResult.score > 0) {
        m_Scoreboard.AddScore(eatResult.score); 
    }
    m_Fruit.OnBeansEaten(eatResult.beansEaten);
    m_Fruit.Update(m_Pacman.GetPosition(), m_Scoreboard);
    m_Fruit.UpdateScoreText();

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
            const int ghostEatScore = m_GhostManager.GetGhostEatScore();
            m_Scoreboard.AddScore(ghostEatScore);
            ShowGhostEatScore(m_GhostManager.GetLastEatenGhostPosition(), ghostEatScore);
            m_GhostManager.SetLastEatenGhostVisible(false);
            m_GhostManager.PauseAnimations();
            m_Pacman.PauseAnimation();
            m_GhostEatPauseTimer = 0.0f;
            m_CurrentState = State::EATING_GHOST;
        }
    }
    HandleExitInput();
}

void App::EatingGhost() {
    if (HandleLevelShortcut()) {
        return;
    }

    m_GhostEatPauseTimer += Util::Time::GetDeltaTimeMs() / 1000.0f;
    m_Pacman.UpdateQueuedDirection();
    m_Fruit.UpdateScoreText();
    UpdateGhostEatScoreText();
    DrawGameplay();

    if (m_GhostEatPauseTimer >= m_GhostEatPauseDuration) {
        m_GhostManager.SetLastEatenGhostVisible(true);
        m_GhostEatPauseTimer = 0.0f;
        m_CurrentState = State::UPDATE;
    }

    HandleExitInput();
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

    HandleExitInput();
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
        m_GameOverTimer = 0.0f;
        return;
    }

    DrawGameover();
    m_GameText->Draw();

    m_GameOverTimer += Util::Time::GetDeltaTimeMs() / 1000.0f;

    //差不多兩秒
    if(m_GameOverTimer > 2.0f){
        m_CurrentState = State::END;
    }
}

void App::DrawGameText(const std::string& message) {
    m_GameText = std::make_shared<Util::GameObject>();
    m_GameText->SetDrawable(std::make_shared<Util::Text>(
        RESOURCE_DIR"/font/inkfree.ttf",
        25,
        message,
        Util::Color::FromName(Util::Colors::YELLOW)
    ));
    m_GameText->m_Transform.translation = {0.0f, 0.0f};
}

void App::DrawVictory() {
    DrawGameText("Victory(Press Tab to NextLevel)");
}

void App::DrawDead() {
    DrawGameText("You dead. (Press Tab to Continue)");
}

void App::DrawGameover(){
    DrawGameText("Game Over!!!");
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
