#include "Scoreboard.hpp"
#include "Util/Text.hpp"
#include "Util/Color.hpp"

void Scoreboard::Start() {
    m_ScoreText = std::make_shared<Util::GameObject>();
    m_ScoreText->SetDrawable(std::make_shared<Util::Text>(
        RESOURCE_DIR"/font/inkfree.ttf", 
        40, 
        "SCORE: 0", 
        Util::Color::FromName(Util::Colors::YELLOW)
    ));
    m_ScoreText->m_Transform.translation = {-450.0f, 250.0f};

    m_LevelText = std::make_shared<Util::GameObject>();
    m_LevelText->SetDrawable(std::make_shared<Util::Text>(
        RESOURCE_DIR"/font/inkfree.ttf", 
        40, 
        "LEVEL: 1", 
        Util::Color::FromName(Util::Colors::WHITE)
    ));
    m_LevelText->m_Transform.translation = {-450.0f, 180.0f};

    m_LivesText = std::make_shared<Util::GameObject>();
    m_LivesText->SetDrawable(std::make_shared<Util::Text>(
        RESOURCE_DIR"/font/inkfree.ttf", 
        40, 
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
        40, 
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
    m_LevelText->SetDrawable(std::make_shared<Util::Text>(
        RESOURCE_DIR"/font/inkfree.ttf", 
        40, 
        "LEVEL: " + std::to_string(m_Level), 
        Util::Color::FromName(Util::Colors::WHITE)
    ));
}


void Scoreboard::UpdateScoreText() {
    m_ScoreText->SetDrawable(std::make_shared<Util::Text>(
        RESOURCE_DIR"/font/inkfree.ttf", 
        40, 
        "SCORE: " + std::to_string(m_Score), 
        Util::Color::FromName(Util::Colors::YELLOW)
    ));
}

int Scoreboard::GetLives(){
    return m_Lives;
}

void Scoreboard::ResetLives(){
    m_Lives = 3;
}
