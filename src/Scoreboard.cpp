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
};

void Scoreboard::AddScore(int points) {
    m_Score += points;
    UpdateScoreText();
}

void Scoreboard::Draw() {
    m_ScoreText->Draw();
    m_LevelText->Draw();
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



