#ifndef LEVEL_CONFIG_HPP
#define LEVEL_CONFIG_HPP

#include <vector>

struct LevelConfig{
    //地圖編號
    int mapIndex;
    //鬼速度
    float ghostSpeed;
    //玩家速度
    float pacmanSpeed;
    //鬼的受驚時間
    float frightenedDuration;
    //鬼的釋放時間
    std::vector<float> release;
};


#endif