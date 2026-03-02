#pragma once
#include "Enemy.hpp"

//=============================================================
//                              ZAKO
//=============================================================

class StdEnemy : public Enemy
{
private:
    float angle = 90;
    int specialCooldown = GetRandomValue(300, 3600);
    int type = GetRandomValue(1, 2);
    inline static int attackCooldown = 360;

public:
    int scoreValue = 50;
    inline static bool attackInProgress = false;

    StdEnemy(float x, float y) : Enemy(x, y)
    {
        this->cooldown = GetRandomValue(240, 1380);
        this->health = 2;
    }

    StdEnemy(float x, float y, bool newSpawn) : Enemy(x, y)
    {
        this->cooldown = GetRandomValue(300, 1380);
        this->health = 2;
        this->spawning = newSpawn;
    }

    void draw() override;
    void update(std::pair<float, float> pos, HitBox target) override;
    void attack(HitBox target) override;

    static void attackReset()
    {
        if (attackInProgress)
        {
            attackCooldown--;
        }
        else
        {
            attackCooldown = 360;
        }

        if (attackCooldown <= 0)
        {
            attackInProgress = false;
            attackCooldown = 360;
        }
    }
    void onDeath();
    int getScore() override { return scoreValue; }
};