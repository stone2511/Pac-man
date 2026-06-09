#include "Ghost_Inky.hpp"

Ghost_Inky::Ghost_Inky(glm::vec2 worldPos) 
    : Ghost("inky", worldPos) {
}

void Ghost_Inky::Update(const Map& map, glm::vec2 pacmanPos, Direction pacmanDir, glm::vec2 blinkyPos, GhostState state) {
    
    //Inky演算法最為複雜
    //是依據Blinky位置到玩家前方兩個單位為向量
    //將該向量延長1倍的終點為target計算最短路徑
    glm::vec2 targetPos = {999.0f, -999.0f};
    if (state == GhostState::CHASE) {
        const float offset = 2.0f * 32.0f;
        glm::vec2 pivotPos = pacmanPos;

        if (pacmanDir == Direction::UP) { pivotPos.y += offset; }
        else if (pacmanDir == Direction::DOWN) { pivotPos.y -= offset; }
        else if (pacmanDir == Direction::LEFT) { pivotPos.x -= offset; }
        else if (pacmanDir == Direction::RIGHT) { pivotPos.x += offset; }

        const glm::vec2 vectorFromBlinky = pivotPos - blinkyPos;
        targetPos = blinkyPos + (vectorFromBlinky * 2.0f);
    }

    UpdateMovement(map, targetPos, state);
}
