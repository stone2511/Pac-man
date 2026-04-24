#include "Ghost.hpp"
#include "Util/Image.hpp"
#include <limits>
#include <random>

Ghost::Ghost(const std::string& texturePath, glm::vec2 worldPos) {
    m_GhostObj = std::make_shared<Util::GameObject>();
    m_NormalDrawable = std::make_shared<Util::Image>(texturePath);
    m_EyesDrawable = std::make_shared<Util::Image>(
        RESOURCE_DIR"/Image/ghost/frightened/eyes0.png"
    );
    m_FrightenedAnimation = std::make_shared<Util::Animation>(
        std::vector<std::string>{
            RESOURCE_DIR"/Image/ghost/frightened/frightened0.png",
            RESOURCE_DIR"/Image/ghost/frightened/frightened1.png"
        },
        true,
        120,
        true,
        0
    );
    m_FrightenedWarningAnimation = std::make_shared<Util::Animation>(
        std::vector<std::string>{
            RESOURCE_DIR"/Image/ghost/frightened/frightened0.png",
            RESOURCE_DIR"/Image/ghost/frightened/frightened1.png",
            RESOURCE_DIR"/Image/ghost/frightened/frightened2.png",
            RESOURCE_DIR"/Image/ghost/frightened/frightened3.png"
        },
        true,
        120,
        true,
        0
    );
    m_GhostObj->SetDrawable(m_NormalDrawable);
    m_SpawnPos = worldPos;
    m_HomePos = worldPos;
    m_GhostObj->m_Transform.translation = m_SpawnPos;
    m_GhostObj->SetZIndex(15);
}

void Ghost::Draw() {
    m_GhostObj->Draw();
}

glm::vec2 Ghost::GetPosition() const {
    return m_GhostObj->m_Transform.translation;
}

void Ghost::Reset(){
    m_GhostObj->m_Transform.translation = m_SpawnPos;
    m_GhostObj->SetDrawable(m_NormalDrawable);
    m_FrightenedAnimation->Pause();
    m_FrightenedAnimation->SetCurrentFrame(0);
    m_FrightenedWarningAnimation->Pause();
    m_FrightenedWarningAnimation->SetCurrentFrame(0);
    m_FrightenedTimeRemaining = 0.0f;
    m_IgnoreFrightened = false;
    m_CurrentDir = Direction::LEFT;
    m_HouseState = HouseState::IN_HOUSE;
}

void Ghost::BecomeEaten() {
    m_GhostObj->SetVisible(true);
    m_GhostObj->SetDrawable(m_EyesDrawable);
    m_FrightenedAnimation->Pause();
    m_FrightenedAnimation->SetCurrentFrame(0);
    m_FrightenedWarningAnimation->Pause();
    m_FrightenedWarningAnimation->SetCurrentFrame(0);
    m_HouseState = HouseState::RETURNING;
    m_FrightenedTimeRemaining = 0.0f;
    m_IgnoreFrightened = true;
    m_CurrentDir = Direction::NONE;
}

void Ghost::ReverseDirection() {
    m_CurrentDir = OppositeDirection(m_CurrentDir);
}

void Ghost::SetFrightenedTimeRemaining(float timeRemaining) {
    m_FrightenedTimeRemaining = timeRemaining;
}

void Ghost::ResetFrightenedImmunity() {
    m_IgnoreFrightened = false;
}

bool Ghost::IgnoresFrightened() const {
    return m_IgnoreFrightened;
}

bool Ghost::IsReturningToHouse() const {
    return m_HouseState == HouseState::RETURNING;
}

void Ghost::SetIsActive(bool active) {
    m_IsActive = active;
}

bool Ghost::IsActive() const {
    return m_IsActive;
}

//控制這隻鬼要不要顯示在畫面上。讓死亡流程中可以統一把鬼隱藏。
void Ghost::SetVisible(bool visible) {
    m_GhostObj->SetVisible(visible);
}

//回傳鬼目前是不是可見，讓碰撞檢查時能忽略已經消失的鬼。
bool Ghost::IsVisible() const {
    return m_GhostObj->GetVisible();
}

void Ghost::SetHouseState(HouseState s) {
    m_HouseState = s;
}

HouseState Ghost::GetHouseState() const{
    return m_HouseState;
}

void Ghost::SetHomePosition(glm::vec2 homePos) {
    m_HomePos = homePos;
}

void Ghost::UpdateMovement(const Map& map, glm::vec2 targetPos, GhostState state) {
    UpdateDrawableForState(state);

    auto pos = m_GhostObj->m_Transform.translation;
    if (HandleExitHouse(map, pos)) {
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
    }

    if (m_CurrentDir != Direction::NONE && CanMoveDirection(map, pos, m_CurrentDir, speed)) {
        pos += GetDirectionVector(m_CurrentDir, speed);
    }
    map.TryWrapTunnel(pos, 16.0f);
    m_GhostObj->m_Transform.translation = pos;
}

void Ghost::UpdateDrawableForState(GhostState state) {
    if (state == GhostState::EATEN) {
        m_FrightenedAnimation->Pause();
        m_FrightenedAnimation->SetCurrentFrame(0);
        m_FrightenedWarningAnimation->Pause();
        m_FrightenedWarningAnimation->SetCurrentFrame(0);
        m_GhostObj->SetDrawable(m_EyesDrawable);
        return;
    }

    if (state == GhostState::FRIGHTENED) {
        auto frightenedDrawable = m_FrightenedAnimation;
        if (m_FrightenedTimeRemaining <= 3.0f) {
            frightenedDrawable = m_FrightenedWarningAnimation;
            m_FrightenedAnimation->Pause();
            m_FrightenedAnimation->SetCurrentFrame(0);
        } else {
            m_FrightenedWarningAnimation->Pause();
            m_FrightenedWarningAnimation->SetCurrentFrame(0);
        }

        if (m_GhostObj->GetVisible()) {
            frightenedDrawable->Play();
        }
        m_GhostObj->SetDrawable(frightenedDrawable);
        return;
    }

    m_FrightenedAnimation->Pause();
    m_FrightenedAnimation->SetCurrentFrame(0);
    m_FrightenedWarningAnimation->Pause();
    m_FrightenedWarningAnimation->SetCurrentFrame(0);
    m_GhostObj->SetDrawable(m_NormalDrawable);
}

bool Ghost::HandleExitHouse(const Map& map, glm::vec2& pos) {
    if (m_HouseState != HouseState::EXITING) {
        return false;
    }

    const glm::vec2 doorPos = map.GridToWorld(10, 7);
    const float exitSpeed = m_NormalSpeed;

    if (std::abs(pos.x - doorPos.x) > exitSpeed) {
        pos.x += (pos.x < doorPos.x) ? exitSpeed : -exitSpeed;
        return true;
    }

    pos.x = doorPos.x;
    if (std::abs(pos.y - doorPos.y) > exitSpeed) {
        pos.y += (pos.y < doorPos.y) ? exitSpeed : -exitSpeed;
        return true;
    }

    pos.y = doorPos.y;
    m_HouseState = HouseState::OUTSIDE;
    return false;
}

bool Ghost::HandleReturnToHouse(const Map& map, glm::vec2& pos) {
    if (m_HouseState != HouseState::RETURNING) {
        return false;
    }

    const glm::vec2 entrancePos = map.GridToWorld(10.0f, 7.0f);
    const glm::vec2 houseLanePos = map.GridToWorld(10.0f, 9.0f);
    const float snapTolerance = 16.0f;
    const bool nearEntrance =
        std::abs(pos.x - entrancePos.x) <= snapTolerance &&
        std::abs(pos.y - entrancePos.y) <= snapTolerance;
    const bool insideDoorLane =
        std::abs(pos.x - entrancePos.x) < 0.01f &&
        pos.y <= entrancePos.y;
    const float minHomeX = std::min(houseLanePos.x, m_HomePos.x) - m_EatenSpeed;
    const float maxHomeX = std::max(houseLanePos.x, m_HomePos.x) + m_EatenSpeed;
    const bool insideHouseLane =
        std::abs(pos.y - houseLanePos.y) <= m_EatenSpeed &&
        pos.x >= minHomeX &&
        pos.x <= maxHomeX;

    if (!nearEntrance && !insideDoorLane && !insideHouseLane) {
        return false;
    }

    if (!insideDoorLane && !insideHouseLane) {
        pos = entrancePos;
    }

    if (std::abs(pos.y - houseLanePos.y) > m_EatenSpeed) {
        pos.y += (pos.y < houseLanePos.y) ? m_EatenSpeed : -m_EatenSpeed;
        return true;
    }

    pos.y = houseLanePos.y;
    if (std::abs(pos.x - m_HomePos.x) > m_EatenSpeed) {
        pos.x += (pos.x < m_HomePos.x) ? m_EatenSpeed : -m_EatenSpeed;
        return true;
    }

    pos = m_HomePos;
    m_GhostObj->SetDrawable(m_NormalDrawable);
    m_HouseState = HouseState::EXITING;
    m_CurrentDir = Direction::UP;
    return true;
}

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

bool Ghost::CanMoveDirection(const Map& map, glm::vec2 pos, Direction direction, float distance) const {
    if (direction == Direction::NONE) {
        return false;
    }

    const float radius = 14.0f;
    const glm::vec2 nextPos = pos + GetDirectionVector(direction, distance);

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
