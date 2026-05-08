#include "Ghost.hpp"
#include "Util/Image.hpp"
#include <limits>
#include <random>

// 建立鬼的圖像、動畫與初始位置資料。
Ghost::Ghost(const std::string& texturePath, glm::vec2 worldPos) {
    m_GhostObj = std::make_shared<Util::GameObject>();
    m_NormalDrawable = std::make_shared<Util::Image>(texturePath);
    m_EyesDrawable = std::make_shared<Util::Image>(
        RESOURCE_DIR"/Image/ghost/frightened/eyes0.png"
    );
    m_EyesUpDrawable = std::make_shared<Util::Image>(
        RESOURCE_DIR"/Image/ghost/frightened/eye_up.png"
    );
    m_EyesDownDrawable = std::make_shared<Util::Image>(
        RESOURCE_DIR"/Image/ghost/frightened/eye_down.png"
    );
    m_EyesLeftDrawable = std::make_shared<Util::Image>(
        RESOURCE_DIR"/Image/ghost/frightened/eye_left.png"
    );
    m_EyesRightDrawable = std::make_shared<Util::Image>(
        RESOURCE_DIR"/Image/ghost/frightened/eye_right.png"
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

// 繪製鬼物件。
void Ghost::Draw() {
    m_GhostObj->Draw();
}

// 取得鬼目前的世界座標。
glm::vec2 Ghost::GetPosition() const {
    return m_GhostObj->m_Transform.translation;
}

// 重設鬼的狀態、動畫與位置，供重新開始一命或重開關卡時使用。
void Ghost::Reset(){
    m_GhostObj->m_Transform.translation = m_SpawnPos;
    m_GhostObj->SetDrawable(m_NormalDrawable);
    m_FrightenedAnimation->Pause();
    m_FrightenedAnimation->SetCurrentFrame(0);
    m_FrightenedWarningAnimation->Pause();
    m_FrightenedWarningAnimation->SetCurrentFrame(0);
    m_FrightenedTimeRemaining = 0.0f;
    m_IgnoreFrightened = false;
    m_HasEnteredHouseDoor = false;
    m_CurrentDir = Direction::LEFT;
    m_HouseState = HouseState::IN_HOUSE;
}

// 把鬼切換成被吃掉後的狀態，之後會用眼睛型態返回鬼屋。
void Ghost::BecomeEaten() {
    m_GhostObj->SetVisible(true);
    m_GhostObj->SetDrawable(GetEyesDrawableForDirection(m_CurrentDir));
    m_FrightenedAnimation->Pause();
    m_FrightenedAnimation->SetCurrentFrame(0);
    m_FrightenedWarningAnimation->Pause();
    m_FrightenedWarningAnimation->SetCurrentFrame(0);
    m_HouseState = HouseState::RETURNING;
    m_FrightenedTimeRemaining = 0.0f;
    m_IgnoreFrightened = true;
    m_HasEnteredHouseDoor = false;
}

// 將目前移動方向反轉，主要用在 frightened 觸發時的立即掉頭。
void Ghost::ReverseDirection() {
    m_CurrentDir = OppositeDirection(m_CurrentDir);
}

// 只刷新外觀狀態，不移動鬼；讓還在鬼屋等待釋放的鬼也能顯示 frightened 動畫。
void Ghost::RefreshDrawable(GhostState state, float frightenedTimeRemaining) {
    m_FrightenedTimeRemaining = frightenedTimeRemaining;
    UpdateDrawableForState(state);
}

// 更新 frightened 剩餘時間，讓閃爍動畫可以依時間切換。
void Ghost::SetFrightenedTimeRemaining(float timeRemaining) {
    m_FrightenedTimeRemaining = timeRemaining;
}

// 清除「忽略 frightened」旗標，讓鬼能再次被 power pellet 影響。
void Ghost::ResetFrightenedImmunity() {
    m_IgnoreFrightened = false;
}

// 回傳這隻鬼目前是否不受 frightened 狀態影響。
bool Ghost::IgnoresFrightened() const {
    return m_IgnoreFrightened;
}

// 判斷鬼是否正處於返回鬼屋的流程中。
bool Ghost::IsReturningToHouse() const {
    return m_HouseState == HouseState::RETURNING;
}

// 設定這隻鬼是否已經被釋放並開始參與遊戲更新。
void Ghost::SetIsActive(bool active) {
    m_IsActive = active;
}

// 回傳這隻鬼是否已經啟用。
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

// 直接設定鬼目前在鬼屋流程中的狀態。
void Ghost::SetHouseState(HouseState s) {
    m_HouseState = s;
}

// 取得鬼目前在鬼屋流程中的狀態。
HouseState Ghost::GetHouseState() const{
    return m_HouseState;
}

// 設定這隻鬼回到鬼屋後應該停留的 home 位置。
void Ghost::SetHomePosition(glm::vec2 homePos) {
    m_HomePos = homePos;
}

// 根據目標點與當前狀態執行鬼的共用移動流程。
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
        if (state == GhostState::EATEN) {
            UpdateDrawableForState(state);
        }
    }

    if (m_CurrentDir != Direction::NONE && CanMoveDirection(map, pos, m_CurrentDir, speed)) {
        pos += GetDirectionVector(m_CurrentDir, speed);
    }
    map.TryWrapTunnel(pos, 16.0f);
    m_GhostObj->m_Transform.translation = pos;
}

// 依照狀態切換普通、frightened、閃爍警告與眼睛素材。
void Ghost::UpdateDrawableForState(GhostState state) {
    if (state == GhostState::EATEN) {
        m_FrightenedAnimation->Pause();
        m_FrightenedAnimation->SetCurrentFrame(0);
        m_FrightenedWarningAnimation->Pause();
        m_FrightenedWarningAnimation->SetCurrentFrame(0);
        m_GhostObj->SetDrawable(GetEyesDrawableForDirection(m_CurrentDir));
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

// 根據眼睛前進方向選擇對應圖片；沒有方向時沿用舊的預設眼睛圖。
std::shared_ptr<Core::Drawable> Ghost::GetEyesDrawableForDirection(Direction direction) const {
    switch (direction) {
        case Direction::UP:
            return m_EyesUpDrawable;
        case Direction::DOWN:
            return m_EyesDownDrawable;
        case Direction::LEFT:
            return m_EyesLeftDrawable;
        case Direction::RIGHT:
            return m_EyesRightDrawable;
        case Direction::NONE:
        default:
            return m_EyesDrawable;
    }
}

// 處理鬼從鬼屋移動到門口並正式出門的流程。
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
    m_GhostObj->SetDrawable(m_NormalDrawable);
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
