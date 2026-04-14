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
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();

    void Update();

    void Reset();

    void Dead();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

private:
    void ValidTask();

    void DrawVictory();

    void DrawDead();

private:
    State m_CurrentState = State::START;
    std::shared_ptr<Util::GameObject> m_GameText;
    int points = 0;

    Map m_Map;
    Pacman m_Pacman;
    Scoreboard m_Scoreboard;
    GhostManager m_GhostManager;
};

#endif
