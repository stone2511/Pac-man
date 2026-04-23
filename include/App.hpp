#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export

#include "Util/GameObject.hpp"
#include "Util/Text.hpp"
#include "Util/Color.hpp"

#include "Map.hpp"
#include "Pacman.hpp"
#include "Scoreboard.hpp"
#include "GhostManager.hpp"

class App {
public:
    enum class State {
        START,
        UPDATE,
        RESET,
        DEAD,
        GAMEOVER,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }
    // 目前狀態的Getter

    void Start();

    void Update();

    void Reset();   //遊戲的重置

    void Dead();    //死亡

    void Gameover();    //遊戲結束

    void End(); // NOLINT(readability-convert-member-functions-to-static)

private:
    void ValidTask();

    void DrawVictory(); //勝利字串

    void DrawDead();    //死亡字串

    void DrawGameover();    //遊戲結束字串

private:
    State m_CurrentState = State::START;

    std::shared_ptr<Util::GameObject> m_GameText;

    //分數和計時器
    int points = 0;
    float GameOverTimer = 0.0f;

    //成員呼叫
    Map m_Map;
    Pacman m_Pacman;
    Scoreboard m_Scoreboard;
    GhostManager m_GhostManager;
};

#endif
