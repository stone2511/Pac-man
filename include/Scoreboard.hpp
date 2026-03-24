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
    void NextLevel();

private:
    int m_Score = 0;
    int m_Level = 1;

    std::shared_ptr<Util::GameObject> m_ScoreText;
    std::shared_ptr<Util::GameObject> m_LevelText;
    
    void UpdateScoreText();
};

#endif