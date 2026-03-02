#pragma once
#include "Enemy.hpp"

//=============================================================
//                         OGAWAMUSHI (SASORI)
//=============================================================

class StEnemy : public Enemy
{
private:
    float angle = 90;

public:
    StEnemy(float x, float y) : Enemy(x, y)
    {
        this->cooldown = GetRandomValue(90, 600);
    }

    StEnemy(float x, float y, bool newSpawn) : Enemy(x, y)
    {
        this->cooldown = GetRandomValue(120, 600);
        this->spawning = newSpawn;
    }

    void draw() override;
    void update(std::pair<float, float> pos, HitBox target) override;
    void attack(HitBox target) override;
    void onDeath();
};