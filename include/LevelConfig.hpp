#ifndef LEVEL_CONFIG_HPP
#define LEVEL_CONFIG_HPP

#include <vector>

struct LevelConfig{
    int mapIndex;
    float ghostSpeed;
    float pacmanSpeed;
    float frightenedDuration;
    std::vector<float> release;
};


#endif