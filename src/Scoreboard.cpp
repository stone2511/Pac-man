#include "Scoreboard.hpp"
#include "Util/Text.hpp"
#include "Util/Color.hpp"

void Scoreboard::Start() {
    m_ScoreText = std::make_shared<Util::GameObject>();
    m_ScoreText->SetDrawable(std::make_shared<Util::Text>(
        RESOURCE_DIR"/font/inkfree.ttf", 
        20, 
        "SCORE: 0", 
        Util::Color::FromName(Util::Colors::YELLOW)
    ));
    m_ScoreText->m_Transform.translation = {-450.0f, 250.0f};

    m_LevelText = std::make_shared<Util::GameObject>();
    m_LevelText->SetDrawable(std::make_shared<Util::Text>(
        RESOURCE_DIR"/font/inkfree.ttf", 
        20, 
        "LEVEL: 1", 
        Util::Color::FromName(Util::Colors::WHITE)
    ));
    m_LevelText->m_Transform.translation = {-450.0f, 180.0f};

    m_LivesText = std::make_shared<Util::GameObject>();
    m_LivesText->SetDrawable(std::make_shared<Util::Text>(
        RESOURCE_DIR"/font/inkfree.ttf", 
        20, 
        "Lives: 3" , 
        Util::Color::FromName(Util::Colors::WHITE)
    ));
    m_LivesText->m_Transform.translation = {-450.0f, 110.0f};
};

void Scoreboard::AddScore(int points) {
    m_Score += points;
    UpdateScoreText();
}

void Scoreboard::MinusLives(){
    m_Lives--;
    m_LivesText->SetDrawable(std::make_shared<Util::Text>(
        RESOURCE_DIR"/font/inkfree.ttf", 
        20, 
        "Lives: " + std::to_string(m_Lives), 
        Util::Color::FromName(Util::Colors::WHITE)
    ));
}

void Scoreboard::Draw() {
    m_ScoreText->Draw();
    m_LevelText->Draw();
    m_LivesText->Draw();
}


void Scoreboard::NextLevel() {
    m_Level++;
    SetLevel(m_Level);
}

void Scoreboard::SetLevel(int level) {
    m_Level = level;
    m_LevelText->SetDrawable(std::make_shared<Util::Text>(
        RESOURCE_DIR"/font/inkfree.ttf", 
        20, 
        "LEVEL: " + std::to_string(m_Level), 
        Util::Color::FromName(Util::Colors::WHITE)
    ));
}

int Scoreboard::GetLevel(){
    return m_Level;
}


void Scoreboard::UpdateScoreText() {
    m_ScoreText->SetDrawable(std::make_shared<Util::Text>(
        RESOURCE_DIR"/font/inkfree.ttf", 
        20, 
        "SCORE: " + std::to_string(m_Score), 
        Util::Color::FromName(Util::Colors::YELLOW)
    ));
}

int Scoreboard::GetLives(){
    return m_Lives;
}

void Scoreboard::ResetLives(){
    m_Lives = 3;
    m_LivesText->SetDrawable(std::make_shared<Util::Text>(
        RESOURCE_DIR"/font/inkfree.ttf", 
        20, 
        "Lives: " + std::to_string(m_Lives), 
        Util::Color::FromName(Util::Colors::WHITE)
    ));
}
