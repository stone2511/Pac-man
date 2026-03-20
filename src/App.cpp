#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

void App::Start() {
    LOG_TRACE("Start");

    m_Map = {
        {1, 1, 1, 1, 1},
        {1, 2, 2, 2, 1},
        {1, 2, 1, 2, 1},
        {1, 2, 2, 2, 1},
        {1, 1, 1, 1, 1}
    };

    // 2. 測試：把地圖印在終端機 (Console) 上
    LOG_INFO("=== 小精靈地圖測試 ===");
    for (int y = 0; y < m_Map.size(); y++) {
        std::string rowString = "";
        for (int x = 0; x < m_Map[y].size(); x++) {
            if (m_Map[y][x] == 1) {
                rowString += "W "; // 牆壁 (Wall)
            } else if (m_Map[y][x] == 2) {
                rowString += ". "; // 豆子
            } else {
                rowString += "  "; // 空地
            }
        }
        // 印出這一行的地圖
        LOG_INFO(rowString); 
    }
    LOG_INFO("======================");

    m_CurrentState = State::UPDATE;
}

void App::Update() {



    /*
     * Do not touch the code below as they serve the purpose for
     * closing the window.
     */
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
