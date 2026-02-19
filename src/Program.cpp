#include "Program.hpp"

Program::Program() {
    // Player ship movement bounds.
    // Player is 10 units tall.
    Background::sideWalls = std::pair<HitBox, HitBox>{ 
        HitBox(0, 0, 10, GetScreenHeight()), 
        HitBox(GetScreenWidth() - 10, 0, 10, GetScreenHeight())
    };
    SpawnEnemiesOnGameStart();
}

void Program::Update() {
    for (Animation& a : Animation::animations) a.update();
    for (int i = 0; i < Animation::animations.size(); i++) {
        if (Animation::animations[i].done) Animation::animations.erase(Animation::animations.begin() + i);
    }
    pauseFrames = std::max(pauseFrames - 1, 0);

    if (!startup && !paused && !gameOver && pauseFrames <= 0) {
        Enemy::ManageEnemies(player->hitBox);
        StdEnemy::attackReset();
        ManageEnemyRespawns();
        player->update();

        for (std::pair<std::pair<float, float>, Enemy*> p : Enemy::enemies) {
            if (p.second && HitBox::Collision(player->hitBox, p.second->hitBox)) {
                Animation::animations.push_back(
                    Animation(player->position.first, player->position.second, 16, 0, 33, 34, 30 ,30, 3, ImageManager::SpriteSheet)
                );

                PlaySound(SoundManager::gameOver);
                Projectile::projectiles.clear();
                player->position.first = GetScreenWidth() / 2 - 15;
                p.second->health = 0;
                pauseFrames = 120;
                lives--;
            }
        }

        for (Projectile& p : Projectile::projectiles) { 
            p.update(); 

            // Verify if enemy projectile collisioned with player
            if(p.ID != 0 && HitBox::Collision(p.getHitBox(), player->hitBox)){

                PlayerReset();
            }

        }

        if (lives <= 0 && pauseFrames <= 0) gameOver = true;
        Projectile::CleanProjectiles();
        Projectile::ProjectileCollision();
    }
}

void Program::Draw() {
    background.Draw();
    if (pauseFrames <= 0 && !gameOver) player->draw();
    for (Animation& a : Animation::animations) a.draw();

    for (int i = 0; i < lives; i++) {
         DrawTexturePro(ImageManager::SpriteSheet, Rectangle{0, 0, 17, 18}, 
                   Rectangle{10.0f + i * 30, GetScreenHeight() - 30.0f, 20, 20}, 
                   Vector2{0, 0}, 0, WHITE);
    }


    for (Projectile p : Projectile::projectiles) p.draw();
    for (std::pair<std::pair<float, float>, Enemy*>& p : Enemy::enemies) if (p.second) p.second->draw();

    if (startup) DrawStartup();
    if (paused) DrawPauseScreen();
    if (gameOver) DrawGameOver();
}

void Program::ManageEnemyRespawns() {
    delay = std::max(delay - 1, 0);

    respawnCooldown -= 1;
    if (respawnCooldown <= 0) {
        respawnCooldown = 1080;
        for (std::pair<std::pair<float, float>, Enemy*>& p : Enemy::enemies) {
            if (!p.second && p.first.second != 150) {
                int eType = GetRandomValue(1, 3);

                if (eType == 1) {
                    p.second = new StEnemy(GetScreenWidth() / 2 - 15, 0, true);
                    respawnCooldown /= 2;
                } else {
                    p.second = new StdEnemy(GetScreenWidth() / 2 - 15, 0, true);
                }

                respawns++;
                break;
            } else if (!p.second && p.first.second == 150) {
                p.second = new SpEnemy(GetScreenWidth() / 2 - 15, 0, true);
                respawns++;
                break;
            }
        }
    }

    if(respawns >= 4) {
        count = 4;
        respawns = 0;
    }

    if (count > 0 && delay <= 0) {
        Enemy::enemies.push_back(std::pair<std::pair<float, float>, Enemy*> {
            std::pair<float, float>{0, 0}, 
            new DyEnemy(GetScreenWidth(), 300)
        });

        count--;
        delay = 20;
    }
}

void Program::DrawStartup() {
    DrawRectangle(0, 0, (float)GetScreenWidth(), (float)GetScreenHeight(), Color{0, 0, 0, 125});
    DrawText("Galaga", (GetScreenWidth() / 2 - 237), 75, 144, WHITE);
    DrawText("Press Enter", (GetScreenWidth() / 2) - 75, GetScreenHeight() / 2, 24, GRAY);
}

void Program::DrawPauseScreen() {
    DrawRectangle(0, 0, (float)GetScreenWidth(), (float)GetScreenHeight(), Color{0, 0, 0, 125});
    DrawText("Paused", (GetScreenWidth() / 2) - 85, GetScreenHeight() / 2 - 60, 48, WHITE);
    DrawText("Press Enter", (GetScreenWidth() / 2) - 75, GetScreenHeight() / 2, 24, GRAY);
}

void Program::DrawGameOver() {
    DrawRectangle(0, 0, (float)GetScreenWidth(), (float)GetScreenHeight(), Color{0, 0, 0, 125});
    DrawText("Game Over", (GetScreenWidth() / 2) - 380, 50, 144, WHITE);
    DrawText("Press Enter", (GetScreenWidth() / 2) - 75, GetScreenHeight() / 2, 24, GRAY);
}

void Program::KeyInputs() {
    if ((!gameOver && !startup && IsKeyPressed('P')) || (paused && IsKeyPressed(KEY_ENTER))) paused = !paused;
    if (!paused && !startup && IsKeyPressed('O')) gameOver = !gameOver;
    if (!gameOver && !paused && IsKeyPressed('I')) startup = !startup;
    if (IsKeyPressed('H')) HitBox::drawHitbox = !HitBox::drawHitbox;
    
    if (gameOver && IsKeyPressed(KEY_ENTER)) {
        gameOver = false;
        Reset();
    }

    if (startup && IsKeyPressed(KEY_ENTER)) {
        startup = false;
    }

    if (!startup && !paused && !gameOver && pauseFrames <= 0) player->keyInputs();
   
}

void Program::PlayerReset() {
    Animation::animations.push_back(
        Animation(player->position.first, player->position.second, 16, 0, 33, 34, 30 ,30, 3, ImageManager::SpriteSheet)
    );

    PlaySound(SoundManager::gameOver);
    Projectile::projectiles.clear();
    player->position.first = GetScreenWidth() / 2 - 15;
    pauseFrames = 120;
    lives--;
}

void Program::Reset() {
    Enemy::enemies.clear();
    SpawnEnemiesOnGameStart();
    StdEnemy::attackInProgress = false;
    player = new Player((GetScreenWidth() / 2) - 15, GetScreenHeight() * 0.75f);
    respawnCooldown = 1080;
    respawns = 0;
    count = 0;
    delay = 0;
    lives = 3;
}

void Program::SpawnEnemiesOnGameStart() {
    Enemy::enemies.push_back(std::pair<std::pair<float, float>, Enemy*> {
            std::pair<float, float>{350, 150}, 
            new SpEnemy(350, 150)
        });

    Enemy::enemies.push_back(std::pair<std::pair<float, float>, Enemy*> {
            std::pair<float, float>{600, 150}, 
            new SpEnemy(600, 150)
        });

    for (int i = 0; i < 30; i++) {

        // x resett
        float x = 250 + 50 * (i % 10);

        // y increase 
        float y = 200;

        if(i > 9 && i <= 19){
            y += 50;
        }

        else if(i > 19){
            y += 100;
        }
        
        Enemy::enemies.push_back(std::pair<std::pair<float, float>, Enemy*> {
            std::pair<float, float>{x, y}, 
            new StdEnemy(x, y)
        });
    }
}