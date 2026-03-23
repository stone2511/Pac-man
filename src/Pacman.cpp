#include "Pacman.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include <cmath>

void Pacman::Start() {
    m_Pacman = std::make_shared<Util::GameObject>();
    m_Pacman->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/Image/character/player.png"));
    
    //Pacman Init Position
    m_Pacman->m_Transform.translation = {-288.0f, -288.0f};
    m_Pacman->SetZIndex(10);
}

void Pacman::Update(const Map& map) {
    //Pacman coordinate
    auto pos = m_Pacman->m_Transform.translation;

    //Move Logic
    //---------------------------------
    //Up and Down Judgement
    glm::vec2 nextPosY = pos;
    if (Util::Input::IsKeyPressed(Util::Keycode::W)) 
    { nextPosY.y += m_Speed; }
    if (Util::Input::IsKeyPressed(Util::Keycode::S)) 
    { nextPosY.y -= m_Speed; }

    //Check if there is any colliding
    if (!IsColliding(map, glm::vec2(pos.x, nextPosY.y))) 
    { pos.y = nextPosY.y;}
    //---------------------------------
    
    //---------------------------------
    //Right and Left Judgement
    glm::vec2 nextPosX = pos;
    if (Util::Input::IsKeyPressed(Util::Keycode::A))
    { nextPosX.x -= m_Speed; }
    if (Util::Input::IsKeyPressed(Util::Keycode::D))
    { nextPosX.x += m_Speed; }

    //Check if there is any colliding
    if (!IsColliding(map, glm::vec2(nextPosX.x, pos.y)))
    { pos.x = nextPosX.x;}
    //---------------------------------

    //Move
    m_Pacman->m_Transform.translation = pos;
}

void Pacman::Draw() {
    m_Pacman->Draw();
}

bool Pacman::IsColliding(const Map& map, glm::vec2 pos) {
    //Radius of the entity
    float radius = 14.0f; 

    return map.IsWall(pos.x - radius, pos.y + radius) || // 左上角
           map.IsWall(pos.x + radius, pos.y + radius) || // 右上角
           map.IsWall(pos.x - radius, pos.y - radius) || // 左下角
           map.IsWall(pos.x + radius, pos.y - radius) || // 右下角
           map.IsDoor(pos.x - radius, pos.y - radius) ||
           map.IsDoor(pos.x + radius, pos.y - radius);
}
