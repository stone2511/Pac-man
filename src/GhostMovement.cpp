#include "Ghost.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <random>

// 根據目標點與當前狀態執行鬼的共用移動流程。
void Ghost::UpdateMovement(const Map& map, glm::vec2 targetPos, GhostState state) {
    UpdateDrawableForState(state);

    auto pos = m_GhostObj->m_Transform.translation;

    map.TryWrapTunnel(pos, 16.0f);

    if (HandleExitHouse(map, pos, state)) {
        m_GhostObj->m_Transform.translation = pos;
        return;
    }

    if (HandleReturnToHouse(map, pos)) {
        m_GhostObj->m_Transform.translation = pos;
        return;
    }

    if (state == GhostState::EATEN) {
        targetPos = map.GridToWorld(10.0f, 7.0f);
    }

    const float speed =
        (state == GhostState::EATEN) ? m_EatenSpeed :
        (state == GhostState::FRIGHTENED) ? m_FrightenedSpeed :
        m_NormalSpeed;
    const glm::vec2 gridCenter = map.GetClosestGridCenter(pos.x, pos.y);
    const bool isNearGridCenter =
        std::abs(pos.x - gridCenter.x) < speed && std::abs(pos.y - gridCenter.y) < speed;
    const bool canKeepMoving =
        m_CurrentDir != Direction::NONE && CanMoveDirection(map, pos, m_CurrentDir, speed);

    if (state == GhostState::FRIGHTENED) {
        if (!canKeepMoving) {
            pos = gridCenter;
            m_CurrentDir = ChooseRandomDirection(map, pos, m_CurrentDir);
        }
    } else if (isNearGridCenter || !canKeepMoving) {
        pos = gridCenter;
        if (HandleReturnToHouse(map, pos)) {
            m_GhostObj->m_Transform.translation = pos;
            return;
        }
        m_CurrentDir = ChooseNextDirection(map, pos, targetPos, state);
        if (state == GhostState::EATEN) {
            UpdateDrawableForState(state);
        }
    }

    if (m_CurrentDir != Direction::NONE && CanMoveDirection(map, pos, m_CurrentDir, speed)) {
        float remainingDistance = speed;
        constexpr float maxStepDistance = 2.0f;
        while (remainingDistance > 0.0f) {
            const float stepDistance = std::min(remainingDistance, maxStepDistance);
            if (!CanMoveDirection(map, pos, m_CurrentDir, stepDistance)) {
                break;
            }

            pos += GetDirectionVector(m_CurrentDir, stepDistance);
            map.TryWrapTunnel(pos, 14.0f);
            remainingDistance -= stepDistance;
        }
    }
    map.TryWrapTunnel(pos, 14.0f);
    UpdateDrawableForState(state);
    m_GhostObj->m_Transform.translation = pos;
}

// 處理鬼從鬼屋移動到門口並正式出門的流程。
bool Ghost::HandleExitHouse(const Map& map, glm::vec2& pos, GhostState state) {
    if (m_HouseState != HouseState::EXITING) {
        return false;
    }

    const glm::vec2 doorPos = map.GridToWorld(10, 7);
    const float exitSpeed = m_NormalSpeed;

    if (std::abs(pos.x - doorPos.x) > exitSpeed) {
        m_CurrentDir = pos.x < doorPos.x ? Direction::RIGHT : Direction::LEFT;
        UpdateDrawableForState(state);
        pos.x += (pos.x < doorPos.x) ? exitSpeed : -exitSpeed;
        return true;
    }

    pos.x = doorPos.x;
    if (std::abs(pos.y - doorPos.y) > exitSpeed) {
        m_CurrentDir = pos.y < doorPos.y ? Direction::UP : Direction::DOWN;
        UpdateDrawableForState(state);
        pos.y += (pos.y < doorPos.y) ? exitSpeed : -exitSpeed;
        return true;
    }

    pos.y = doorPos.y;
    m_HouseState = HouseState::OUTSIDE;
    return false;
}

// 處理鬼被吃掉後回到鬼屋、回家定位與重新出門的流程。
bool Ghost::HandleReturnToHouse(const Map& map, glm::vec2& pos) {
    if (m_HouseState != HouseState::RETURNING) {
        return false;
    }

    const glm::vec2 entrancePos = map.GridToWorld(10.0f, 7.0f);
    const glm::vec2 houseLanePos = map.GridToWorld(10.0f, 9.0f);
    const float snapTolerance = m_EatenSpeed;
    const bool nearEntrance =
        std::abs(pos.x - entrancePos.x) <= snapTolerance &&
        std::abs(pos.y - entrancePos.y) <= snapTolerance;
    const bool insideDoorLane =
        std::abs(pos.x - entrancePos.x) < 0.01f &&
        pos.y <= entrancePos.y &&
        pos.y >= houseLanePos.y;
    const float minHomeX = std::min(houseLanePos.x, m_HomePos.x) - m_EatenSpeed;
    const float maxHomeX = std::max(houseLanePos.x, m_HomePos.x) + m_EatenSpeed;
    const bool insideHouseLane =
        m_HasEnteredHouseDoor &&
        std::abs(pos.y - houseLanePos.y) <= m_EatenSpeed &&
        pos.x >= minHomeX &&
        pos.x <= maxHomeX;

    if (!nearEntrance && !insideDoorLane && !insideHouseLane) {
        return false;
    }

    if (!insideDoorLane && !insideHouseLane) {
        pos = entrancePos;
        m_HasEnteredHouseDoor = true;
    }

    if (insideDoorLane) {
        m_HasEnteredHouseDoor = true;
    }

    if (std::abs(pos.y - houseLanePos.y) > m_EatenSpeed) {
        m_CurrentDir = pos.y < houseLanePos.y ? Direction::UP : Direction::DOWN;
        m_GhostObj->SetDrawable(GetEyesDrawableForDirection(m_CurrentDir));
        pos.y += (pos.y < houseLanePos.y) ? m_EatenSpeed : -m_EatenSpeed;
        return true;
    }

    pos.y = houseLanePos.y;
    if (std::abs(pos.x - m_HomePos.x) > m_EatenSpeed) {
        m_CurrentDir = pos.x < m_HomePos.x ? Direction::RIGHT : Direction::LEFT;
        m_GhostObj->SetDrawable(GetEyesDrawableForDirection(m_CurrentDir));
        pos.x += (pos.x < m_HomePos.x) ? m_EatenSpeed : -m_EatenSpeed;
        return true;
    }

    pos = m_HomePos;
    m_CurrentAnimation = GetNormalAnimationForDirection(Direction::UP);
    m_GhostObj->SetDrawable(m_CurrentAnimation);
    m_HouseState = HouseState::EXITING;
    m_HasEnteredHouseDoor = false;
    m_CurrentDir = Direction::UP;
    return true;
}

// 在路口依照目標點、優先序與不可回頭規則選出下一步方向。
Direction Ghost::ChooseNextDirection(const Map& map, glm::vec2 pos, glm::vec2 targetPos, GhostState state) const {
    auto directions = GetValidDirections(map, pos);

    if (directions.empty()) {
        return OppositeDirection(m_CurrentDir);
    }

    if (state == GhostState::FRIGHTENED) {
        if (std::find(directions.begin(), directions.end(), m_CurrentDir) != directions.end()) {
            return m_CurrentDir;
        }

        static std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<std::size_t> dist(0, directions.size() - 1);
        return directions[dist(rng)];
    }

    if (directions.size() > 1) {
        const Direction reverseDir = OppositeDirection(m_CurrentDir);
        directions.erase(
            std::remove(directions.begin(), directions.end(), reverseDir),
            directions.end()
        );
        if (directions.empty()) {
            directions.push_back(reverseDir);
        }
    }

    Direction bestDirection = directions.front();
    float bestDistance = std::numeric_limits<float>::max();

    const std::array<Direction, 4> priority = {
        Direction::UP,
        Direction::LEFT,
        Direction::DOWN,
        Direction::RIGHT
    };

    for (const Direction direction : priority) {
        if (std::find(directions.begin(), directions.end(), direction) == directions.end()) {
            continue;
        }

        const glm::vec2 nextPos = pos + GetDirectionVector(direction, 32.0f);
        const float distance =
            std::pow(nextPos.x - targetPos.x, 2) +
            std::pow(nextPos.y - targetPos.y, 2);

        if (distance < bestDistance) {
            bestDistance = distance;
            bestDirection = direction;
        }
    }

    return bestDirection;
}

// 在 frightened 等需要隨機移動時，從合法方向中挑出下一步。
Direction Ghost::ChooseRandomDirection(const Map& map, glm::vec2 pos, Direction previousDirection) const {
    auto directions = GetValidDirections(map, pos);
    if (directions.empty()) {
        return OppositeDirection(previousDirection);
    }

    if (directions.size() > 1 && previousDirection != Direction::NONE) {
        const Direction reverseDirection = OppositeDirection(previousDirection);
        directions.erase(
            std::remove(directions.begin(), directions.end(), reverseDirection),
            directions.end()
        );

        if (directions.empty()) {
            directions.push_back(reverseDirection);
        }
    }

    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<std::size_t> dist(0, directions.size() - 1);
    return directions[dist(rng)];
}

// 列出目前格點上所有可前進的方向。
std::vector<Direction> Ghost::GetValidDirections(const Map& map, glm::vec2 pos) const {
    std::vector<Direction> directions;

    if (CanMoveDirection(map, pos, Direction::UP, 16.0f)) {
        directions.push_back(Direction::UP);
    }

    if (CanMoveDirection(map, pos, Direction::DOWN, 16.0f)) {
        directions.push_back(Direction::DOWN);
    }

    if (CanMoveDirection(map, pos, Direction::LEFT, 16.0f)) {
        directions.push_back(Direction::LEFT);
    }

    if (CanMoveDirection(map, pos, Direction::RIGHT, 16.0f)) {
        directions.push_back(Direction::RIGHT);
    }

    return directions;
}

// 檢查鬼往指定方向前進一段距離後，是否會撞牆或撞到不可通行的門。
bool Ghost::CanMoveDirection(const Map& map, glm::vec2 pos, Direction direction, float distance) const {
    if (direction == Direction::NONE) {
        return false;
    }

    const float radius = 14.0f;
    glm::vec2 nextPos = pos + GetDirectionVector(direction, distance);
    map.TryWrapTunnel(nextPos, radius);

    const bool hitsWall =
        map.IsWall(nextPos.x - radius, nextPos.y + radius) ||
        map.IsWall(nextPos.x + radius, nextPos.y + radius) ||
        map.IsWall(nextPos.x - radius, nextPos.y - radius) ||
        map.IsWall(nextPos.x + radius, nextPos.y - radius);

    const bool allowDoor =
        m_HouseState == HouseState::RETURNING && direction == Direction::DOWN;
    const bool hitsDoor =
        !allowDoor &&
        (map.IsDoor(nextPos.x - radius, nextPos.y - radius) ||
         map.IsDoor(nextPos.x + radius, nextPos.y - radius));

    return !hitsWall && !hitsDoor;
}

// 把方向與速度換算成實際位移向量。
glm::vec2 Ghost::GetDirectionVector(Direction direction, float speed) {
    switch (direction) {
        case Direction::UP:
            return {0.0f, speed};
        case Direction::DOWN:
            return {0.0f, -speed};
        case Direction::LEFT:
            return {-speed, 0.0f};
        case Direction::RIGHT:
            return {speed, 0.0f};
        case Direction::NONE:
        default:
            return {0.0f, 0.0f};
    }
}

// 取得指定方向的相反方向。
Direction Ghost::OppositeDirection(Direction direction) {
    switch (direction) {
        case Direction::UP:
            return Direction::DOWN;
        case Direction::DOWN:
            return Direction::UP;
        case Direction::LEFT:
            return Direction::RIGHT;
        case Direction::RIGHT:
            return Direction::LEFT;
        case Direction::NONE:
        default:
            return Direction::NONE;
    }
}
