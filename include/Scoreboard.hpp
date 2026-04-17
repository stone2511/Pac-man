#ifndef SCOREBOARD_HPP
#define SCOREBOARD_HPP

#include "pch.hpp"
#include "Util/GameObject.hpp"
#include <memory>
#include <string>

class Scoreboard {
public:
    void Start();
    void Draw();

    void AddScore(int points); 
    void MinusLives();
    void NextLevel();

    int GetLives();
    void ResetLives();

private:
    int m_Score = 0;
    int m_Level = 1;
    int m_Lives = 3;

    std::shared_ptr<Util::GameObject> m_ScoreText;
    std::shared_ptr<Util::GameObject> m_LevelText;
    std::shared_ptr<Util::GameObject> m_LivesText;
    
    void UpdateScoreText();
};

#endif