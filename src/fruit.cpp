#include "fruit.hpp"

#include "Util/Color.hpp"
#include "Util/Image.hpp"
#include "Util/Text.hpp"
#include "Util/Time.hpp"

#include <cmath>

void Fruit::Reset(int level, const Map& map) {
    m_BeansEatenThisLevel = 0;
    m_HasSpawnedThisLevel = false;
    m_IsVisible = false;
    m_Timer = 0.0f;
    m_ScoreTextTimer = 0.0f;
    m_ScoreText = nullptr;
    LoadForLevel(level, map);
}

void Fruit::Hide() {
    m_IsVisible = false;
}

void Fruit::OnBeansEaten(int beansEaten) {
    m_BeansEatenThisLevel += beansEaten;

    if (!m_HasSpawnedThisLevel && m_BeansEatenThisLevel >= m_SpawnBeanCount) {
        Spawn();
    }
}

void Fruit::Update(const glm::vec2& pacmanPosition, Scoreboard& scoreboard) {
    if (!m_IsVisible || m_Fruit == nullptr) {
        return;
    }

    m_Timer += Util::Time::GetDeltaTimeMs() / 1000.0f;
    if (m_Timer >= m_VisibleDuration) {
        m_IsVisible = false;
        return;
    }

    const glm::vec2 fruitPosition = m_Fruit->m_Transform.translation;
    constexpr float eatRadius = 18.0f;
    if (std::abs(pacmanPosition.x - fruitPosition.x) < eatRadius &&
        std::abs(pacmanPosition.y - fruitPosition.y) < eatRadius) {
        scoreboard.AddScore(m_CurrentScore);
        ShowScore(fruitPosition);
        m_IsVisible = false;
    }
}

void Fruit::UpdateScoreText() {
    if (m_ScoreText == nullptr) {
        return;
    }

    m_ScoreTextTimer += Util::Time::GetDeltaTimeMs() / 1000.0f;
    if (m_ScoreTextTimer >= m_ScoreTextDuration) {
        m_ScoreText = nullptr;
        m_ScoreTextTimer = 0.0f;
    }
}

void Fruit::Draw() {
    if (m_IsVisible && m_Fruit != nullptr) {
        m_Fruit->Draw();
    }
    if (m_ScoreText != nullptr) {
        m_ScoreText->Draw();
    }
}

void Fruit::LoadForLevel(int level, const Map& map) {
    m_CurrentScore = GetScoreForLevel(level);
    m_Fruit = std::make_shared<Util::GameObject>();
    m_Fruit->SetDrawable(std::make_shared<Util::Image>(GetImageForLevel(level)));
    m_Fruit->m_Transform.translation = map.GridToWorld(10.0f, 11.0f);
    m_Fruit->SetZIndex(6);
}

void Fruit::Spawn() {
    if (m_Fruit == nullptr) {
        return;
    }

    m_IsVisible = true;
    m_HasSpawnedThisLevel = true;
    m_Timer = 0.0f;
}

void Fruit::ShowScore(glm::vec2 position) {
    m_ScoreText = std::make_shared<Util::GameObject>();
    m_ScoreText->SetDrawable(std::make_shared<Util::Text>(
        RESOURCE_DIR"/font/inkfree.ttf",
        12,
        std::to_string(m_CurrentScore),
        Util::Color::FromName(Util::Colors::WHITE)
    ));
    m_ScoreText->m_Transform.translation = position;
    m_ScoreText->SetZIndex(20);
    m_ScoreTextTimer = 0.0f;
}

int Fruit::GetScoreForLevel(int level) const {
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

std::string Fruit::GetImageForLevel(int level) const {
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
