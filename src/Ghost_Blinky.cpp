#include "Ghost_Blinky.hpp"
#include <algorithm>
#include <cmath>
#include <limits>

Ghost_Blinky::Ghost_Blinky(glm::vec2 worldPos)
    : Ghost("Blinky", RESOURCE_DIR"/Image/ghost/blinky0.png", 'B', worldPos) {
    SetDirection(Direction::RIGHT);
}

void Ghost_Blinky::Update(const Map& map,
                          glm::vec2 pacmanPos,
                          Direction pacmanDir,
                          glm::vec2 blinkyPos,
                          GhostState state) {
    (void)pacmanDir;
    (void)blinkyPos;

    if (UpdateHouseRelease()) {
        return;
    }

    auto pos = GetPosition();
    const auto gridCenter = map.GetClosestGridCenter(pos.x, pos.y);

    glm::vec2 targetPos = pacmanPos;
    if (state == GhostState::SCATTER) {
        // Blinky 的散開目標改成地圖右上角附近的格子中心，路徑會更穩定。
        targetPos = map.GridToWorld(19.0f, 1.0f);
    }

    // 沿用原本 Blinky 的寫法：到格子中心時，比較四個方向到目標的距離。
    if (IsAtGridCenter(map, pos)) {
        pos = gridCenter;

        float distUp = std::numeric_limits<float>::max();
        float distDown = std::numeric_limits<float>::max();
        float distLeft = std::numeric_limits<float>::max();
        float distRight = std::numeric_limits<float>::max();

        const auto currentDir = GetCurrentDirection();

        if (!map.IsWall(pos.x, pos.y + 32.0f) &&
            !map.IsDoor(pos.x, pos.y + 32.0f) &&
            currentDir != Direction::DOWN) {
            distUp = std::pow(pos.x - targetPos.x, 2) +
                     std::pow((pos.y + 32.0f) - targetPos.y, 2);
        }

        if (!map.IsWall(pos.x, pos.y - 32.0f) &&
            !map.IsDoor(pos.x, pos.y - 32.0f) &&
            currentDir != Direction::UP) {
            distDown = std::pow(pos.x - targetPos.x, 2) +
                       std::pow((pos.y - 32.0f) - targetPos.y, 2);
        }

        if (!map.IsWall(pos.x - 32.0f, pos.y) &&
            !map.IsDoor(pos.x - 32.0f, pos.y) &&
            currentDir != Direction::RIGHT) {
            distLeft = std::pow((pos.x - 32.0f) - targetPos.x, 2) +
                       std::pow(pos.y - targetPos.y, 2);
        }

        if (!map.IsWall(pos.x + 32.0f, pos.y) &&
            !map.IsDoor(pos.x + 32.0f, pos.y) &&
            currentDir != Direction::LEFT) {
            distRight = std::pow((pos.x + 32.0f) - targetPos.x, 2) +
                        std::pow(pos.y - targetPos.y, 2);
        }

        const float minDist = std::min({distUp, distLeft, distDown, distRight});

        if (minDist == distUp) {
            SetDirection(Direction::UP);
        } else if (minDist == distLeft) {
            SetDirection(Direction::LEFT);
        } else if (minDist == distDown) {
            SetDirection(Direction::DOWN);
        } else if (minDist == distRight) {
            SetDirection(Direction::RIGHT);
        } else {
            // 如果四個方向都被擋住，就允許回頭，避免卡住。
            const auto reverseDirection = GetOppositeDirection(currentDir);
            const auto reverseTilePos = pos + GetTileOffset(reverseDirection);

            if (!map.IsWall(reverseTilePos.x, reverseTilePos.y) &&
                !map.IsDoor(reverseTilePos.x, reverseTilePos.y)) {
                SetDirection(reverseDirection);
            }
        }
    }

    auto nextPos = pos + GetMoveOffset(GetCurrentDirection());
    map.TryWrapTunnel(nextPos, GetCollisionRadius());

    if (!IsColliding(map, nextPos)) {
        pos = nextPos;
    }

    SetPosition(pos);
}
