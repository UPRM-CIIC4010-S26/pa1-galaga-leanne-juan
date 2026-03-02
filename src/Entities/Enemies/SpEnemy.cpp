#include "SpEnemy.hpp"

void SpEnemy::draw()
{
    if (HitBox::drawHitbox)
        this->hitBox.draw();
    if (health > 2)
    {
        if (!frame)
        {
            DrawTexturePro(ImageManager::SpriteSheet, Rectangle{1, 36, 17, 18},
                           Rectangle{this->position.first, this->position.second, 30, 30},
                           Vector2{0, 0}, 0, WHITE);
        }
        else
        {
            DrawTexturePro(ImageManager::SpriteSheet, Rectangle{19, 36, 17, 18},
                           Rectangle{this->position.first, this->position.second, 30, 30},
                           Vector2{0, 0}, 0, WHITE);
        }
    }
    else
    {
        if (!frame)
        {
            DrawTexturePro(ImageManager::SpriteSheet, Rectangle{1, 54, 17, 18},
                           Rectangle{this->position.first, this->position.second, 30, 30},
                           Vector2{0, 0}, 0, WHITE);
        }
        else
        {
            DrawTexturePro(ImageManager::SpriteSheet, Rectangle{19, 54, 17, 18},
                           Rectangle{this->position.first, this->position.second, 30, 30},
                           Vector2{0, 0}, 0, WHITE);
        }
    }
}

void SpEnemy::update(std::pair<float, float> pos, HitBox target)
{
    frameChange();

    if (!spawning && this->specialCooldown > 0)
    {
        this->position.first = pos.first;
        this->position.second = pos.second;
        this->hitBox.box.x = pos.first;
        this->hitBox.box.y = pos.second;
        this->cooldown--;
        this->specialCooldown--;
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
        this->attack(target);
        this->attackInProgress = true;
    }

    if (this->cooldown <= 0)
    {
        Projectile::projectiles.push_back(Projectile(Projectile(position.first + 15, position.second, 1)));
        PlaySound(SoundManager::shoot);
        this->cooldown = GetRandomValue(360, 1380);
        this->attackInProgress = false;
    }
}

void SpEnemy::attack(HitBox target)
{
    if (this->angle < 270)
    {
        this->position.first += sqrt(3) * cos(this->angle * M_PI / 180);
        this->position.second -= sqrt(3) * sin(this->angle * M_PI / 180);
        this->hitBox.box.x = this->position.first;
        this->hitBox.box.y = this->position.second;
        this->angle += 4;
    }
    else
    {
        if (target.box.y - this->position.second > 200)
        {
            float xFact =
                ((target.box.x + target.box.width / 2) - this->position.first) /
                Math::getDistance(this->position, std::pair<float, float>{target.box.x, target.box.y});
            this->position.first += xFact;
            this->position.second += 3;
            this->hitBox.box.x = this->position.first;
            this->hitBox.box.y = this->position.second;
        }
        else
        {
            if (!this->stop)
            {
                Projectile::projectiles.push_back(Projectile(this->position.first + this->hitBox.box.width / 2, this->position.second, 1));
                Projectile::projectiles.push_back(Projectile((this->position.first + this->hitBox.box.width / 2) + 5, this->position.second, 45, 1));
                Projectile::projectiles.push_back(Projectile((this->position.first + this->hitBox.box.width / 2) - 5, this->position.second, 135, 1));
                PlaySound(SoundManager::shoot);
                this->delay = 30;
                this->stop = true;
            }
            else
            {
                delay--;
            }

            if (delay <= 0)
            {
                this->angle = 90;
                this->spawning = true;
                this->stop = false;
                this->specialCooldown = GetRandomValue(1380, 3600);
            }
        }
    }
}
void SpEnemy::onDeath()
{
    std::cout << "SpEnemy died" << std::endl;
}