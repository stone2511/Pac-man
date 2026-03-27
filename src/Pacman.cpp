#include "Pacman.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include <cmath>

void Pacman::Start() {
    m_Pacman = std::make_shared<Util::GameObject>();
    m_Pacman->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/Image/character/player.png"));
    
    //Pacman Init Position
    m_Pacman->m_Transform.translation = {0.0f, -160.0f};
    m_Pacman->SetZIndex(10);
}

int Pacman::Update(Map& map) {
    //Pacman coordinate
    auto pos = m_Pacman->m_Transform.translation;

    if (Util::Input::IsKeyDown(Util::Keycode::W)) {
        m_QueuedDirection = Direction::Up;
    } else if (Util::Input::IsKeyDown(Util::Keycode::S)) {
        m_QueuedDirection = Direction::Down;
    } else if (Util::Input::IsKeyDown(Util::Keycode::A)) {
        m_QueuedDirection = Direction::Left;
    } else if (Util::Input::IsKeyDown(Util::Keycode::D)) {
        m_QueuedDirection = Direction::Right;
    }

    // Turn as soon as the requested direction becomes available.
    const auto queuedPos = pos + GetDirectionOffset(m_QueuedDirection);
    if (m_QueuedDirection != Direction::None &&
        !IsColliding(map, queuedPos)) {
        m_CurrentDirection = m_QueuedDirection;
    }

    // Keep moving in the current direction until a wall blocks the path.
    auto nextPos = pos + GetDirectionOffset(m_CurrentDirection);
    map.TryWrapTunnel(nextPos, 14.0f);
    if (m_CurrentDirection != Direction::None &&
        !IsColliding(map, nextPos)) {
        pos = nextPos;
    }

    //Move
    m_Pacman->m_Transform.translation = pos;

    int score = map.CheckAndEatBeans(pos);
    return score;
}

void Pacman::Draw() {
    m_Pacman->Draw();
}

bool Pacman::IsColliding(Map& map, glm::vec2 pos) {
    //Radius of the entity
    float radius = 14.0f;

    glm::vec2 wrappedPos = pos;
    if (map.TryWrapTunnel(wrappedPos, radius)) {
        return false;
    }

    return map.IsWall(pos.x - radius, pos.y + radius) || // 左上角
           map.IsWall(pos.x + radius, pos.y + radius) || // 右上角
           map.IsWall(pos.x - radius, pos.y - radius) || // 左下角
           map.IsWall(pos.x + radius, pos.y - radius) || // 右下角
           map.IsDoor(pos.x - radius, pos.y - radius) ||
           map.IsDoor(pos.x + radius, pos.y - radius);
}

glm::vec2 Pacman::GetDirectionOffset(Direction direction) const {
    switch (direction) {
        case Direction::Up:
            return {0.0f, m_Speed};
        case Direction::Down:
            return {0.0f, -m_Speed};
        case Direction::Left:
            return {-m_Speed, 0.0f};
        case Direction::Right:
            return {m_Speed, 0.0f};
        case Direction::None:
        default:
            return {0.0f, 0.0f};
    }
}
