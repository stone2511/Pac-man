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

#include <string>

class App {
public:
    enum class State {
        START,
        UPDATE,
        RESET,
        DYING,
        DEAD,
        GAMEOVER,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }
    // 目前狀態的Getter

    void Start();

    void Update();

    void Reset();   //遊戲的重置

    void Dying();   //死亡前置動畫

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
    void SpawnFruit();
    void UpdateFruit();
    void ShowFruitScore(glm::vec2 position);
    void UpdateFruitScoreText();
    void DrawFruit();
    void LoadFruitForLevel();
    int GetFruitScoreForLevel(int level) const;
    std::string GetFruitImageForLevel(int level) const;
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
    int m_BeansEatenThisLevel = 0;
    bool m_HasFruitSpawnedThisLevel = false;
    bool m_IsFruitVisible = false;
    float m_FruitTimer = 0.0f;
    float m_FruitScoreTextTimer = 0.0f;
    int m_CurrentFruitScore = 100;
    std::shared_ptr<Util::GameObject> m_Fruit;
    std::shared_ptr<Util::GameObject> m_FruitScoreText;

    static constexpr int m_FruitSpawnBeanCount = 70;
    static constexpr float m_FruitVisibleDuration = 12.0f;
    static constexpr float m_FruitScoreTextDuration = 1.0f;

    //成員呼叫
    Map m_Map;
    Pacman m_Pacman;
    Scoreboard m_Scoreboard;
    GhostManager m_GhostManager;
};

#endif
