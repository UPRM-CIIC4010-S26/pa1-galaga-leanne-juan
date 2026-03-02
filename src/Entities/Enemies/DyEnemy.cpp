#include "DyEnemy.hpp"

void DyEnemy::draw()
{
    if (HitBox::drawHitbox)
        this->hitBox.draw();
    
        // Use random texture 
    DrawTexturePro(ImageManager::SpriteSheet, this->randomTexture,
                   Rectangle{this->position.first + 15, this->position.second + 15, 30, 30},
                   Vector2{15, 15}, this->aimAngle, WHITE);
}

void DyEnemy::update(std::pair<float, float> pos, HitBox target)
{
    this->cooldown--;

    if (!loop)
    {
        this->angle += 0.25;
        this->aimAngle += 0.25;
        if (fmod(this->angle, 180) == 0)
        {
            loop = true;
        }
    }
    else
    {
        this->angle += 2;
        this->aimAngle += 2;
        if (fmod(this->angle, 540) == 0)
        {
            loop = false;
        }
    }

    this->position.first += 3 * cos(angle * M_PI / 180);
    this->position.second += 3 * sin(angle * M_PI / 180);
    this->hitBox.box.x = this->position.first;
    this->hitBox.box.y = this->position.second;

    if (this->cooldown <= 0)
    {
        Projectile::projectiles.push_back(Projectile(Projectile(position.first + 15, position.second, 1)));
        PlaySound(SoundManager::shoot);
        this->cooldown = GetRandomValue(90, 300);
    }
}

void DyEnemy::attack(HitBox target) { /* Not used */ }
void DyEnemy::onDeath()
{
    std::cout << "DyEnemy died" << std::endl;
}