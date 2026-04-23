#include "Pacman.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include <cmath>

void Pacman::Start() {
    
    Animate();

    m_Pacman = std::make_shared<Util::GameObject>();
    m_Pacman->SetDrawable(m_CurrentAnimation);
    
    //Pacman Init Position
    m_Pacman->m_Transform.translation = {0.0f, -160.0f};
    m_Pacman->SetZIndex(10);

    UpdateAnimation(false);
}

void Pacman::Animate(){
    const auto makeAnimation = [](const std::string& folder,
                                  const std::string& prefix,
                                  const int frameCount,
                                  const bool looping = true) {
        std::vector<std::string> frames;
        frames.reserve(frameCount);
        for (int i = 0; i < frameCount; ++i) {
            frames.push_back(RESOURCE_DIR"/Image/character/" + folder + "/" +
                             prefix + std::to_string(i) + ".png");
        }

        return std::make_shared<Util::Animation>(frames, false, 80, looping, 0);
    };

    m_UpAnimation = makeAnimation("pacman_up", "pacman_up", 6);
    m_DownAnimation = makeAnimation("pacman_down", "pacman_down", 6);
    m_LeftAnimation = makeAnimation("pacman_left", "pacman_left", 6);
    m_RightAnimation = makeAnimation("pacman_right", "pacman_right", 6);
    m_DeathAnimation = makeAnimation("pacman_death", "pacman_death", 13, false);
    m_CurrentAnimation = m_RightAnimation;
    
}

int Pacman::Update(Map& map) {
    //Pacman coordinate
    auto pos = m_Pacman->m_Transform.translation;

    if (Util::Input::IsKeyDown(Util::Keycode::W)) {
        m_QueuedDirection = Direction::UP;
    } else if (Util::Input::IsKeyDown(Util::Keycode::S)) {
        m_QueuedDirection = Direction::DOWN;
    } else if (Util::Input::IsKeyDown(Util::Keycode::A)) {
        m_QueuedDirection = Direction::LEFT;
    } else if (Util::Input::IsKeyDown(Util::Keycode::D)) {
        m_QueuedDirection = Direction::RIGHT;
    }

    // Turn as soon as the requested direction becomes available.
    const auto queuedPos = pos + GetDirectionOffset(m_QueuedDirection);
    if (m_QueuedDirection != Direction::NONE &&
        !IsColliding(map, queuedPos)) {
        m_CurrentDirection = m_QueuedDirection;
    }

    // Keep moving in the current direction until a wall blocks the path.
    auto nextPos = pos + GetDirectionOffset(m_CurrentDirection);
    map.TryWrapTunnel(nextPos, 14.0f);
    bool didMove = false;
    if (m_CurrentDirection != Direction::NONE &&
        !IsColliding(map, nextPos)) {
        pos = nextPos;
        didMove = true;
        m_FacingDirection = m_CurrentDirection;
    }

    //Move
    m_Pacman->m_Transform.translation = pos;
    UpdateAnimation(didMove);

    int score = map.CheckAndEatBeans(pos);
    return score;
}

void Pacman::Draw() {
    m_Pacman->Draw();
}

//暫停動畫，讓 Pacman 保持在當前幀。死亡動畫會切換到專用的動畫並播放，其他時候則根據移動方向切換動畫。
void Pacman::PauseAnimation() {
    if (m_CurrentAnimation != nullptr) {
        m_CurrentAnimation->Pause();
    }
}

//開始死亡動畫，並切換到專用的死亡動畫。這個函式會在 App::StartDeathSequence() 中被呼叫。
void Pacman::StartDeathAnimation() {
    if (m_CurrentAnimation != m_DeathAnimation) {
        PauseAnimation();
        m_DeathAnimation->Pause();
        m_DeathAnimation->SetCurrentFrame(0);
        m_CurrentAnimation = m_DeathAnimation;
        m_Pacman->SetDrawable(m_DeathAnimation);
    }

    if (m_DeathAnimation->GetState() != Util::Animation::State::PLAY) {
        m_DeathAnimation->Play();
    }
}

//回傳死亡動畫是否已經播完，讓 App::Update() 能在適當的時候切換到死亡狀態。
bool Pacman::IsDeathAnimationFinished() const {
    return m_DeathAnimation->GetState() == Util::Animation::State::ENDED;
}

void Pacman::Reset(){
    m_Pacman->m_Transform.translation = {0.0f, -160.0f};
    m_CurrentDirection = Direction::NONE;
    m_QueuedDirection = Direction::NONE;
    m_FacingDirection = Direction::RIGHT;
    PauseAnimation();
    m_UpAnimation->SetCurrentFrame(0);
    m_DownAnimation->SetCurrentFrame(0);
    m_LeftAnimation->SetCurrentFrame(0);
    m_RightAnimation->SetCurrentFrame(0);
    m_DeathAnimation->Pause();
    m_DeathAnimation->SetCurrentFrame(0);
    m_CurrentAnimation = m_RightAnimation;
    m_Pacman->SetDrawable(m_CurrentAnimation);
    UpdateAnimation(false);
}

bool Pacman::IsColliding(Map& map, glm::vec2 pos) {

    //Radius of the entity
    float m_radius = 14.0f;
    
    glm::vec2 wrappedPos = pos;
    if (map.TryWrapTunnel(wrappedPos, m_radius)) {
        return false;
    }

    return map.IsWall(pos.x - m_radius, pos.y + m_radius) || // 左上角
           map.IsWall(pos.x + m_radius, pos.y + m_radius) || // 右上角
           map.IsWall(pos.x - m_radius, pos.y - m_radius) || // 左下角
           map.IsWall(pos.x + m_radius, pos.y - m_radius) || // 右下角
           map.IsDoor(pos.x - m_radius, pos.y - m_radius) ||
           map.IsDoor(pos.x + m_radius, pos.y - m_radius);
}

glm::vec2 Pacman::GetDirectionOffset(Direction direction) const {
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

std::shared_ptr<Util::Animation> Pacman::GetAnimation(Direction direction) const {
    switch (direction) {
        case Direction::UP:
            return m_UpAnimation;
        case Direction::DOWN:
            return m_DownAnimation;
        case Direction::LEFT:
            return m_LeftAnimation;
        case Direction::RIGHT:
        case Direction::NONE:
        default:
            return m_RightAnimation;
    }
}

void Pacman::UpdateAnimation(bool isMoving) {
    auto animation = GetAnimation(m_FacingDirection);
    if (m_CurrentAnimation != animation) {
        m_CurrentAnimation = animation;
        m_Pacman->SetDrawable(animation);
    }

    if (isMoving) {
        animation->Play();
        return;
    }

    animation->Pause();
}

glm::vec2 Pacman::GetPosition() const {
    return m_Pacman->m_Transform.translation;
}

Direction Pacman::GetDirection() const {
    return m_CurrentDirection;
}
