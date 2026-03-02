#include "StEnemy.hpp"

void StEnemy::draw()
{
    if (HitBox::drawHitbox)
        this->hitBox.draw();
    DrawTexturePro(ImageManager::SpriteSheet, Rectangle{2, 111, 13, 13},
                   Rectangle{this->position.first, this->position.second, 30, 30},
                   Vector2{0, 0}, 0, WHITE);
}

void StEnemy::update(std::pair<float, float> pos, HitBox target)
{
    if (!spawning)
    {
        this->position.first = pos.first;
        this->position.second = pos.second;
        this->hitBox.box.x = pos.first;
        this->hitBox.box.y = pos.second;
        this->cooldown--;
    }
    else
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

    if (this->cooldown <= 0)
    {
        Projectile::projectiles.push_back(Projectile(Projectile(position.first + +this->hitBox.box.width / 2, position.second, 1)));
        PlaySound(SoundManager::shoot);
        this->cooldown = GetRandomValue(120, 600);
    }
}

void StEnemy::attack(HitBox target) { /* Not used */ }
void StEnemy::onDeath()
{
    std::cout << "StEnemy died" << std::endl;
}