#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export

#include "Util/GameObject.hpp"

#include "Map.hpp"
#include "Pacman.hpp"
#include "Scoreboard.hpp"
#include "GhostManager.hpp"
#include "LevelConfig.hpp"
#include "fruit.hpp"

class App {
public:
    enum class State {
        START,
        UPDATE,
        RESET,
        EATING_GHOST,
        DYING,
        DEAD,
        GAMEOVER,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }
    // 目前狀態的Getter

    LevelConfig GetLevelConfig(int level);

    void Start();

    void Update();

    void Reset();   //遊戲的重置

    void Dying();   //死亡前置動畫

    void EatingGhost();    //吃鬼後的短暫暫停

    void Dead();    //死亡

    void Gameover();    //遊戲結束

    void End(); // NOLINT(readability-convert-member-functions-to-static)

private:
    void DrawGameplay();
    bool HandleLevelShortcut();
    void LoadLevel(int newLevel);
    void StartDeathSequence();
    void ResetDeathSequence();
    void ResetFruit();
    void ShowGhostEatScore(glm::vec2 position, int score);
    void UpdateGhostEatScoreText();
    void ValidTask();

    void DrawVictory(); //勝利字串

    void DrawDead();    //死亡字串

    void DrawGameover();    //遊戲結束字串

private:
    State m_CurrentState = State::START;

    std::shared_ptr<Util::GameObject> m_GameText;

    //分數和計時器
    float GameOverTimer = 0.0f;
    int level = 1;
    float m_DeathSequenceTimer = 0.0f;
    bool m_HasHiddenGhosts = false;
    bool m_HasStartedDeathAnimation = false;
    float m_GhostEatPauseTimer = 0.0f;
    float m_GhostEatScoreTextTimer = 0.0f;
    std::shared_ptr<Util::GameObject> m_GhostEatScoreText;

    static constexpr float m_GhostEatPauseDuration = 0.75f;
    static constexpr float m_GhostEatScoreTextDuration = 0.75f;

    //成員呼叫
    Map m_Map;
    Pacman m_Pacman;
    Scoreboard m_Scoreboard;
    GhostManager m_GhostManager;
    Fruit m_Fruit;
};

#endif
