#include "StdEnemy.hpp"

void StdEnemy::draw()
{
    if (HitBox::drawHitbox)
        this->hitBox.draw();
    switch (this->type)
    {
    case 1:
        if (!frame)
        {
            if (this->health > 1)
            {
                DrawTexturePro(ImageManager::SpriteSheet, Rectangle{2, 75, 15, 12},
                               Rectangle{this->position.first, this->position.second, 30, 30},
                               Vector2{0, 0}, 0, WHITE);
            }
            else
            {
                DrawTexturePro(ImageManager::SpriteSheet, Rectangle{107, 75, 15, 12},
                               Rectangle{this->position.first, this->position.second, 30, 30},
                               Vector2{0, 0}, 0, WHITE);
            }
        }
        else
        {
            if (this->health > 1)
            {
                DrawTexturePro(ImageManager::SpriteSheet, Rectangle{20, 75, 15, 12},
                               Rectangle{this->position.first, this->position.second, 30, 30},
                               Vector2{0, 0}, 0, WHITE);
            }
            else
            {
                DrawTexturePro(ImageManager::SpriteSheet, Rectangle{125, 75, 15, 12},
                               Rectangle{this->position.first, this->position.second, 30, 30},
                               Vector2{0, 0}, 0, WHITE);
            }
        }
        break;

    case 2:
        if (!frame)
        {
            if (this->health > 1)
            {
                DrawTexturePro(ImageManager::SpriteSheet, Rectangle{2, 93, 15, 12},
                               Rectangle{this->position.first, this->position.second, 30, 30},
                               Vector2{0, 0}, 0, WHITE);
            }
            else
            {
                DrawTexturePro(ImageManager::SpriteSheet, Rectangle{107, 93, 15, 12},
                               Rectangle{this->position.first, this->position.second, 30, 30},
                               Vector2{0, 0}, 0, WHITE);
            }
        }
        else
        {
            if (this->health > 1)
            {
                DrawTexturePro(ImageManager::SpriteSheet, Rectangle{20, 93, 15, 12},
                               Rectangle{this->position.first, this->position.second, 30, 30},
                               Vector2{0, 0}, 0, WHITE);
            }
            else
            {
                DrawTexturePro(ImageManager::SpriteSheet, Rectangle{125, 93, 15, 12},
                               Rectangle{this->position.first, this->position.second, 30, 30},
                               Vector2{0, 0}, 0, WHITE);
            }
        }
        break;
    }
}

void StdEnemy::update(std::pair<float, float> pos, HitBox target)
{
    frameChange();
    if (!spawning && this->specialCooldown > 0)
    {
        this->position.first = pos.first;
        this->position.second = pos.second;
        this->hitBox.box.x = pos.first;
        this->hitBox.box.y = pos.second;
        this->cooldown--;
        this->specialCooldown = attackInProgress ? specialCooldown + 1 : specialCooldown - 1;
    }
    else if (spawning)
    {
        float xFact = (pos.first - this->position.first) / Math::getDistance(this->position, pos);
        float yFact = (pos.second - this->position.second) / Math::getDistance(this->position, pos);

        this->position.first += 2 * xFact;
        this->position.second += 2 * yFact;
        this->hitBox.box.x = this->position.first;
        this->hitBox.box.y = this->position.second;

        if (abs(this->position.second - pos.second) <= 1)
        {
            this->position.first = pos.first;
            this->position.second = pos.second;
            this->spawning = false;
        }
    }
    else
    {
        if (!attackInProgress)
            PlaySound(SoundManager::attack);
        attackInProgress = true;
        this->attack(target);
    }

    if (this->cooldown <= 0)
    {
        Projectile::projectiles.push_back(Projectile(Projectile(position.first + +this->hitBox.box.width / 2, position.second, 1)));
        PlaySound(SoundManager::shoot);
        this->cooldown = GetRandomValue(300, 1380);
    }
}

void StdEnemy::attack(HitBox target)
{
    if (this->angle < 270)
    {
        this->position.first += sqrt(5) * cos(this->angle * M_PI / 180);
        this->position.second -= sqrt(5) * sin(this->angle * M_PI / 180);
        this->hitBox.box.x = this->position.first;
        this->hitBox.box.y = this->position.second;
        this->angle += 4;
    }
    else
    {
        float xFact = ((target.box.x + target.box.width / 2) - this->position.first) /
                      Math::getDistance(this->position, std::pair<float, float>{target.box.x, target.box.y});
        this->position.first += 1.5 * xFact;
        this->position.second += 5;
        this->hitBox.box.x = this->position.first;
        this->hitBox.box.y = this->position.second;

        if (this->position.second > GetScreenHeight() + 100)
        {
            this->angle = 90;
            this->position.first = GetScreenWidth() / 2;
            this->position.second = 0;
            this->spawning = true;
            this->specialCooldown = GetRandomValue(300, 3600);
            attackInProgress = false;
        }
    }
}

void StdEnemy::onDeath()
{
    std::cout << "StdEnemy died" << std::endl;
}