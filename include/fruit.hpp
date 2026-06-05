#ifndef FRUIT_HPP
#define FRUIT_HPP

#include "pch.hpp"

#include "Map.hpp"
#include "Scoreboard.hpp"
#include "Util/GameObject.hpp"

#include <memory>
#include <string>

class Fruit {
public:
    void Reset(int level, const Map& map);
    void Hide();
    void OnBeansEaten(int beansEaten);
    void Update(const glm::vec2& pacmanPosition, Scoreboard& scoreboard);
    void UpdateScoreText();
    void Draw();

private:
    void LoadForLevel(int level, const Map& map);
    void Spawn();
    void ShowScore(glm::vec2 position);
    int GetScoreForLevel(int level) const;
    std::string GetImageForLevel(int level) const;

    int m_BeansEatenThisLevel = 0;
    bool m_HasSpawnedThisLevel = false;
    bool m_IsVisible = false;
    float m_Timer = 0.0f;
    float m_ScoreTextTimer = 0.0f;
    int m_CurrentScore = 100;
    std::shared_ptr<Util::GameObject> m_Fruit;
    std::shared_ptr<Util::GameObject> m_ScoreText;

    static constexpr int m_SpawnBeanCount = 70;
    static constexpr float m_VisibleDuration = 12.0f;
    static constexpr float m_ScoreTextDuration = 1.0f;
};

#endif
