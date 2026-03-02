#pragma once
#include "Enemy.hpp"

//=============================================================
//                              EI (MIDORI)
//=============================================================

class DyEnemy : public Enemy
{
private:
    float angle = 135;
    float aimAngle = 225;
    bool loop = false;

public:
    int scoreValue = 160;
    DyEnemy(float x, float y) : Enemy(x, y)
    {
        this->cooldown = GetRandomValue(90, 300);
        this->health = 1;
    }

    void draw() override;
    void update(std::pair<float, float> pos, HitBox target) override;
    void attack(HitBox target) override;
    void onDeath();
    int getScore() override { return scoreValue; }
};