#include "Ghost.hpp"
#include "Util/Image.hpp"
#include "Util/Time.hpp"
#include <array>
#include <cmath>
#include <random>

Ghost::Ghost(const std::string& name,
             const std::string& texturePath,
             char displaySymbol,
             glm::vec2 worldPos)
    : m_Name(name),
      m_DisplaySymbol(displaySymbol),
      m_Rng(std::random_device{}()) {
    m_GhostObj = std::make_shared<Util::GameObject>();
    m_GhostObj->SetDrawable(std::make_shared<Util::Image>(texturePath));
    m_SpawnPos = worldPos;
    m_GhostObj->m_Transform.translation = m_SpawnPos;
    m_GhostObj->SetZIndex(5);
}

void Ghost::Draw() {
    m_GhostObj->Draw();
}

glm::vec2 Ghost::GetPosition() const {
    return m_GhostObj->m_Transform.translation;
}

void Ghost::Reset(){
    m_GhostObj->m_Transform.translation = m_SpawnPos;
}
const std::string& Ghost::GetName() const {
    return m_Name;
}

char Ghost::GetDisplaySymbol() const {
    return m_DisplaySymbol;
}

Ghost::Direction Ghost::GetCurrentDirection() const {
    return m_CurrentDir;
}

void Ghost::SetHouseRelease(float delaySeconds,
                            const std::vector<glm::vec2>& exitPath) {
    m_ReleaseDelay = delaySeconds;
    m_ReleaseTimer = 0.0f;
    m_HouseExitPath = exitPath;
    m_CurrentExitTarget = 0;
    m_IsReleased = exitPath.empty();

    if (!m_IsReleased) {
        SetDirection(Direction::NONE);
    }
}

bool Ghost::UpdateHouseRelease() {
    if (m_IsReleased) {
        return false;
    }

    m_ReleaseTimer += Util::Time::GetDeltaTimeMs() / 1000.0f;
    if (m_ReleaseTimer < m_ReleaseDelay) {
        return true;
    }

    if (m_CurrentExitTarget >= m_HouseExitPath.size()) {
        m_IsReleased = true;
        return false;
    }

    auto pos = GetPosition();
    const auto target = m_HouseExitPath[m_CurrentExitTarget];
    const auto delta = target - pos;
    const auto distance = glm::length(delta);

    if (distance <= m_Speed) {
        pos = target;
        SetPosition(pos);
        ++m_CurrentExitTarget;

        if (m_CurrentExitTarget >= m_HouseExitPath.size()) {
            m_IsReleased = true;
            return false;
        }

        return true;
    }

    const auto step = glm::normalize(delta) * m_Speed;
    pos += step;
    SetPosition(pos);

    if (std::abs(delta.x) > std::abs(delta.y)) {
        SetDirection(delta.x > 0.0f ? Direction::RIGHT : Direction::LEFT);
    } else {
        SetDirection(delta.y > 0.0f ? Direction::UP : Direction::DOWN);
    }

    return true;
}

void Ghost::UpdateRandomMovement(const Map& map) {
    auto pos = GetPosition();

    // 鬼怪走到格子中心時，再重新決定下一個方向，這樣路徑會更穩定。
    if (IsAtGridCenter(map, pos)) {
        pos = map.GetClosestGridCenter(pos.x, pos.y);

        auto directions = GetAvailableDirections(map, pos, false);
        if (directions.empty()) {
            directions = GetAvailableDirections(map, pos, true);
        }

        if (!directions.empty()) {
            SetDirection(PickRandomDirection(directions));
        } else {
            SetDirection(Direction::NONE);
        }
    }

    auto nextPos = pos + GetMoveOffset(m_CurrentDir);
    map.TryWrapTunnel(nextPos, m_CollisionRadius);

    if (m_CurrentDir != Direction::NONE && !IsColliding(map, nextPos)) {
        pos = nextPos;
    } else if (IsAtGridCenter(map, pos)) {
        // 如果前面剛好是牆，就再抽一次方向，避免鬼怪卡住。
        auto directions = GetAvailableDirections(
            map,
            map.GetClosestGridCenter(pos.x, pos.y),
            true);

        if (!directions.empty()) {
            SetDirection(PickRandomDirection(directions));
            nextPos = pos + GetMoveOffset(m_CurrentDir);
            map.TryWrapTunnel(nextPos, m_CollisionRadius);

            if (!IsColliding(map, nextPos)) {
                pos = nextPos;
            }
        }
    }

    SetPosition(pos);
}

bool Ghost::IsColliding(const Map& map, glm::vec2 pos) const {
    glm::vec2 wrappedPos = pos;
    if (map.TryWrapTunnel(wrappedPos, m_CollisionRadius)) {
        return false;
    }

    return map.IsWall(pos.x - m_CollisionRadius, pos.y + m_CollisionRadius) ||
           map.IsWall(pos.x + m_CollisionRadius, pos.y + m_CollisionRadius) ||
           map.IsWall(pos.x - m_CollisionRadius, pos.y - m_CollisionRadius) ||
           map.IsWall(pos.x + m_CollisionRadius, pos.y - m_CollisionRadius) ||
           map.IsDoor(pos.x - m_CollisionRadius, pos.y + m_CollisionRadius) ||
           map.IsDoor(pos.x + m_CollisionRadius, pos.y + m_CollisionRadius) ||
           map.IsDoor(pos.x - m_CollisionRadius, pos.y - m_CollisionRadius) ||
           map.IsDoor(pos.x + m_CollisionRadius, pos.y - m_CollisionRadius);
}

bool Ghost::IsAtGridCenter(const Map& map, glm::vec2 pos) const {
    const auto center = map.GetClosestGridCenter(pos.x, pos.y);
    // 只要「接近」格子中心就吸附回去，維持 speed = 3 時也能穩定轉向。
    // 這裡用 < m_Speed，避免鬼怪剛移動 3px 就立刻又被吸回中心。
    const float tolerance = m_Speed;

    return std::abs(pos.x - center.x) < tolerance &&
           std::abs(pos.y - center.y) < tolerance;
}

glm::vec2 Ghost::GetMoveOffset(Direction direction) const {
    switch (direction) {
        case Direction::UP:
            return {0.0f, m_Speed};
        case Direction::DOWN:
            return {0.0f, -m_Speed};
        case Direction::LEFT:
            return {-m_Speed, 0.0f};
        case Direction::RIGHT:
            return {m_Speed, 0.0f};
        case Direction::NONE:
        default:
            return {0.0f, 0.0f};
    }
}

glm::vec2 Ghost::GetTileOffset(Direction direction) const {
    constexpr float gridStep = 32.0f;

    switch (direction) {
        case Direction::UP:
            return {0.0f, gridStep};
        case Direction::DOWN:
            return {0.0f, -gridStep};
        case Direction::LEFT:
            return {-gridStep, 0.0f};
        case Direction::RIGHT:
            return {gridStep, 0.0f};
        case Direction::NONE:
        default:
            return {0.0f, 0.0f};
    }
}

std::vector<Ghost::Direction> Ghost::GetAvailableDirections(
    const Map& map,
    glm::vec2 centerPos,
    bool allowReverse) const {
    std::vector<Direction> directions;

    const std::array<Direction, 4> candidates = {
        Direction::UP,
        Direction::LEFT,
        Direction::DOWN,
        Direction::RIGHT
    };

    for (const auto direction : candidates) {
        if (!allowReverse && direction == GetOppositeDirection(m_CurrentDir)) {
            continue;
        }

        const auto nextTilePos = centerPos + GetTileOffset(direction);
        if (!map.IsWall(nextTilePos.x, nextTilePos.y) &&
            !map.IsDoor(nextTilePos.x, nextTilePos.y)) {
            directions.push_back(direction);
        }
    }

    return directions;
}

Ghost::Direction Ghost::PickRandomDirection(
    const std::vector<Direction>& directions) {
    if (directions.empty()) {
        return Direction::NONE;
    }

    std::uniform_int_distribution<std::size_t> distribution(
        0,
        directions.size() - 1);
    return directions[distribution(m_Rng)];
}

Ghost::Direction Ghost::GetOppositeDirection(Direction direction) const {
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

void Ghost::SetDirection(Direction direction) {
    m_CurrentDir = direction;
}

void Ghost::SetPosition(glm::vec2 pos) {
    m_GhostObj->m_Transform.translation = pos;
}

float Ghost::GetSpeed() const {
    return m_Speed;
}

float Ghost::GetCollisionRadius() const {
    return m_CollisionRadius;
}
