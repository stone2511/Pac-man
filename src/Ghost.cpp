#include "Ghost.hpp"
#include "Util/Image.hpp"

// 建立鬼的圖像、動畫與初始位置資料。
Ghost::Ghost(const std::string& ghostName, glm::vec2 worldPos) {
    const auto makeAnimation = [&ghostName](const std::string& direction) {
        const std::string basePath =
            std::string(RESOURCE_DIR) + "/Image/ghost/" + ghostName + "/" +
            ghostName + "_" + direction;
        return std::make_shared<Util::Animation>(
            std::vector<std::string>{
                basePath + "0.png",
                basePath + "1.png"
            },
            false,
            120,
            true,
            0
        );
    };

    m_GhostObj = std::make_shared<Util::GameObject>();
    m_UpAnimation = makeAnimation("up");
    m_DownAnimation = makeAnimation("down");
    m_LeftAnimation = makeAnimation("left");
    m_RightAnimation = makeAnimation("right");
    m_CurrentAnimation = m_LeftAnimation;
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
    m_GhostObj->SetDrawable(m_CurrentAnimation);
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
    ResetMovementState();
    ResetSpecialAnimations();
}

// 把鬼切換成被吃掉後的狀態，之後會用眼睛型態返回鬼屋。
void Ghost::BecomeEaten() {
    SetVisible(true);
    PauseNormalAnimations();
    m_GhostObj->SetDrawable(GetEyesDrawableForDirection(m_CurrentDir));
    ResetSpecialAnimations();
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
    if (state != GhostState::FRIGHTENED && state != GhostState::EATEN) {
        PauseNormalAnimations();
    }
}

void Ghost::UpdateHouseIdle(GhostState state, float frightenedTimeRemaining) {
    if (m_HouseState != HouseState::IN_HOUSE) {
        RefreshDrawable(state, frightenedTimeRemaining);
        return;
    }

    m_FrightenedTimeRemaining = frightenedTimeRemaining;
    auto pos = m_GhostObj->m_Transform.translation;
    const float top = m_HomePos.y + m_HouseIdleRange;
    const float bottom = m_HomePos.y - m_HouseIdleRange;

    if (m_HouseIdleDir == Direction::UP) {
        pos.y += m_HouseIdleSpeed;
        if (pos.y >= top) {
            pos.y = top;
            m_HouseIdleDir = Direction::DOWN;
        }
    } else {
        pos.y -= m_HouseIdleSpeed;
        if (pos.y <= bottom) {
            pos.y = bottom;
            m_HouseIdleDir = Direction::UP;
        }
    }

    m_CurrentDir = m_HouseIdleDir;
    m_GhostObj->m_Transform.translation = pos;
    UpdateDrawableForState(state);
}

void Ghost::SetHouseIdleStartDirection(Direction direction) {
    m_HouseIdleStartDir = direction;
    m_HouseIdleDir = direction;
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
    m_IsVisible = visible;
    m_GhostObj->SetVisible(visible);
}

//回傳鬼目前是不是可見，讓碰撞檢查時能忽略已經消失的鬼。
bool Ghost::IsVisible() const {
    return m_IsVisible;
}

void Ghost::SetSpeed(float s) {
    m_NormalSpeed = s;
}

void Ghost::PauseAnimation() {
    if (m_CurrentAnimation != nullptr) {
        m_CurrentAnimation->Pause();
    }
    m_FrightenedAnimation->Pause();
    m_FrightenedWarningAnimation->Pause();
    PauseNormalAnimations();
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

// 依照狀態切換普通、frightened、閃爍警告與眼睛素材。
void Ghost::UpdateDrawableForState(GhostState state) {
    if (state == GhostState::EATEN) {
        PauseNormalAnimations();
        ResetSpecialAnimations();
        m_GhostObj->SetDrawable(GetEyesDrawableForDirection(m_CurrentDir));
        return;
    }

    if (state == GhostState::FRIGHTENED) {
        PauseNormalAnimations();
        auto frightenedDrawable = m_FrightenedAnimation;
        if (m_FrightenedTimeRemaining <= 3.0f) {
            frightenedDrawable = m_FrightenedWarningAnimation;
            m_FrightenedAnimation->Pause();
            m_FrightenedAnimation->SetCurrentFrame(0);
        } else {
            m_FrightenedWarningAnimation->Pause();
            m_FrightenedWarningAnimation->SetCurrentFrame(0);
        }

        if (IsVisible()) {
            frightenedDrawable->Play();
        }
        m_GhostObj->SetDrawable(frightenedDrawable);
        return;
    }

    ResetSpecialAnimations();

    auto normalAnimation = GetNormalAnimationForDirection(m_CurrentDir);
    if (m_CurrentAnimation != normalAnimation) {
        if (m_CurrentAnimation != nullptr) {
            m_CurrentAnimation->Pause();
        }
        m_CurrentAnimation = normalAnimation;
    }

    if (IsVisible()) {
        m_CurrentAnimation->Play();
    }
    m_GhostObj->SetDrawable(m_CurrentAnimation);
}

// 暫停所有普通移動動畫，避免切到特殊狀態時背景繼續更新。
void Ghost::PauseNormalAnimations() {
    m_UpAnimation->Pause();
    m_DownAnimation->Pause();
    m_LeftAnimation->Pause();
    m_RightAnimation->Pause();
}

void Ghost::ResetSpecialAnimations() {
    m_FrightenedAnimation->Pause();
    m_FrightenedAnimation->SetCurrentFrame(0);
    m_FrightenedWarningAnimation->Pause();
    m_FrightenedWarningAnimation->SetCurrentFrame(0);
}

void Ghost::ResetMovementState() {
    PauseNormalAnimations();
    m_UpAnimation->SetCurrentFrame(0);
    m_DownAnimation->SetCurrentFrame(0);
    m_LeftAnimation->SetCurrentFrame(0);
    m_RightAnimation->SetCurrentFrame(0);
    m_CurrentAnimation = m_LeftAnimation;
    m_GhostObj->SetDrawable(m_CurrentAnimation);
    m_FrightenedTimeRemaining = 0.0f;
    m_IgnoreFrightened = false;
    m_HasEnteredHouseDoor = false;
    m_HouseIdleDir = m_HouseIdleStartDir;
    m_CurrentDir = Direction::LEFT;
    m_HouseState = HouseState::IN_HOUSE;
}

// 依照目前方向選擇一般狀態的移動動畫；無方向時沿用最後面向。
std::shared_ptr<Util::Animation> Ghost::GetNormalAnimationForDirection(Direction direction) const {
    switch (direction) {
        case Direction::UP:
            return m_UpAnimation;
        case Direction::DOWN:
            return m_DownAnimation;
        case Direction::RIGHT:
            return m_RightAnimation;
        case Direction::LEFT:
        case Direction::NONE:
        default:
            return m_LeftAnimation;
    }
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
