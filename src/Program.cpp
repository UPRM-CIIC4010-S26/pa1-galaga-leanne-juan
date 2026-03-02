#include "Program.hpp"
#include "raymath.hpp"

Program::Program()
{
    // Player ship movement bounds.
    // Player is 10 units tall.
    Background::sideWalls = std::pair<HitBox, HitBox>{
        HitBox(0, 0, 10, GetScreenHeight()),
        HitBox(GetScreenWidth() - 10, 0, 10, GetScreenHeight())};

    //=========================== Phase 1.3 ===========================
    SpawnEnemiesOnGameStart();
    //=========================== Phase 1.3 ===========================
}

void Program::Update()
{
    for (Animation &a : Animation::animations)
        a.update();
    for (int i = 0; i < Animation::animations.size(); i++)
    {
        if (Animation::animations[i].done)
            Animation::animations.erase(Animation::animations.begin() + i);
    }
    pauseFrames = std::max(pauseFrames - 1, 0);

    if (!startup && !paused && !gameOver && pauseFrames <= 0)
    {
        Enemy::ManageEnemies(player->hitBox);
        for (int pts : Enemy::pendingScores)
        {
            UpdateScore(pts);
        }
        Enemy::pendingScores.clear();
        StdEnemy::attackReset();
        DecreaseCooldownOnDifficulty(score);
        ManageEnemyRespawns();
        player->update();

        for (std::pair<std::pair<float, float>, Enemy *> p : Enemy::enemies)
        {
            if (p.second && HitBox::Collision(player->hitBox, p.second->hitBox))
            {
                Animation::animations.push_back(
                    Animation(player->position.first, player->position.second, 16, 0, 33, 34, 30, 30, 3, ImageManager::SpriteSheet));

                PlaySound(SoundManager::gameOver);
                Projectile::projectiles.clear();
                player->position.first = GetScreenWidth() / 2 - 15;
                p.second->health = 0;
                pauseFrames = 120;
                lives--;
            }
        }

        for (Projectile &p : Projectile::projectiles)
        {
            p.update();

            // Verify if enemy projectile collisioned with player
            if (p.ID != 0 && HitBox::Collision(p.getHitBox(), player->hitBox))
            {

                PlayerReset();
            }
        }

        if (lives <= 0 && pauseFrames <= 0)
            gameOver = true;
        Projectile::CleanProjectiles();
        Projectile::ProjectileCollision();
    }
}

void Program::Draw()
{
    background.Draw();
    if (pauseFrames <= 0 && !gameOver)
        player->draw();
    for (Animation &a : Animation::animations)
        a.draw();

    for (int i = 0; i < lives; i++)
    {
        DrawTexturePro(ImageManager::SpriteSheet, Rectangle{0, 0, 17, 18},
                       Rectangle{10.0f + i * 30, GetScreenHeight() - 30.0f, 20, 20},
                       Vector2{0, 0}, 0, WHITE);
    }

    for (Projectile p : Projectile::projectiles)
        p.draw();
    for (std::pair<std::pair<float, float>, Enemy *> &p : Enemy::enemies)
        if (p.second)
            p.second->draw();

    //+++++++++++++++++++ The Holy Draw Score Function +++++++++++++++++++
    DrawScore(score, 20.0f, 50.0f, WHITE);
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    if (startup)
        DrawStartup();
    if (paused)
        DrawPauseScreen();
    if (gameOver)
        DrawGameOver();
}

void Program::ManageEnemyRespawns()
{
    delay = std::max(delay - 1, 0);

    respawnCooldown -= 1;
    if (respawnCooldown <= 0)
    {
        respawnCooldown = 1080;
        for (std::pair<std::pair<float, float>, Enemy *> &p : Enemy::enemies)
        {
            if (!p.second && p.first.second != 150)
            {
                int eType = GetRandomValue(1, 3);

                if (eType == 1)
                {
                    p.second = new StEnemy(GetScreenWidth() / 2 - 15, 0, true);
                    respawnCooldown /= 2;
                }
                else
                {
                    p.second = new StdEnemy(GetScreenWidth() / 2 - 15, 0, true);
                }

                respawns++;
                break;
            }
            else if (!p.second && p.first.second == 150)
            {
                p.second = new SpEnemy(GetScreenWidth() / 2 - 15, 0, true);
                respawns++;
                break;
            }
        }
    }

    if (respawns >= 4)
    {
        count = 4;
        respawns = 0;
    }

    if (count > 0 && delay <= 0)
    {
        Enemy::enemies.push_back(std::pair<std::pair<float, float>, Enemy *>{
            std::pair<float, float>{0, 0},
            new DyEnemy(GetScreenWidth(), 300)});

        count--;
        delay = 20;
    }
}

void Program::DrawStartup() // Draw start screen
{
    DrawRectangle(0, 0, (float)GetScreenWidth(), (float)GetScreenHeight(), Color{0, 0, 0, 125});
    DrawText("Galaga", (GetScreenWidth() / 2 - 237), 75, 144, WHITE);
    DrawText("Press Enter", (GetScreenWidth() / 2) - 75, GetScreenHeight() / 2, 24, GRAY);
}

void Program::DrawPauseScreen()
{
    DrawRectangle(0, 0, (float)GetScreenWidth(), (float)GetScreenHeight(), Color{0, 0, 0, 125});
    DrawText("Paused", (GetScreenWidth() / 2) - 85, GetScreenHeight() / 2 - 60, 48, WHITE);
    DrawText("Press Enter", (GetScreenWidth() / 2) - 75, GetScreenHeight() / 2, 24, GRAY);
}

void Program::DrawGameOver()
{
    DrawRectangle(0, 0, (float)GetScreenWidth(), (float)GetScreenHeight(), Color{0, 0, 0, 125});
    DrawText("Game Over", (GetScreenWidth() / 2) - 380, 50, 144, WHITE);
    DrawText("Press Enter", (GetScreenWidth() / 2) - 75, GetScreenHeight() / 2, 24, GRAY);
}

void Program::KeyInputs()
{
    if ((!gameOver && !startup && IsKeyPressed('P')) || (paused && IsKeyPressed(KEY_ENTER)))
        paused = !paused;
    if (!paused && !startup && IsKeyPressed('O'))
        gameOver = !gameOver;
    if (!gameOver && !paused && IsKeyPressed('I'))
        startup = !startup;
    if (IsKeyPressed('H'))
        HitBox::drawHitbox = !HitBox::drawHitbox;

    if (gameOver && IsKeyPressed(KEY_ENTER))
    {
        gameOver = false;
        Reset();
    }

    if (startup && IsKeyPressed(KEY_ENTER))
    {
        startup = false;
    }

    if (!startup && !paused && !gameOver && pauseFrames <= 0)
        player->keyInputs();

    if (IsKeyPressed('K'))
    {
        UpdateScore(500);
    }
    if (IsKeyPressed('L'))
    {
        ResetScore();
    }
}

void Program::PlayerReset()
{
    Animation::animations.push_back(
        Animation(player->position.first, player->position.second, 16, 0, 33, 34, 30, 30, 3, ImageManager::SpriteSheet));

    PlaySound(SoundManager::gameOver);
    Projectile::projectiles.clear();
    player->position.first = GetScreenWidth() / 2 - 15;
    pauseFrames = 120;
    lives--;
}

void Program::Reset()
{
    Enemy::enemies.clear();
    SpawnEnemiesOnGameStart();
    StdEnemy::attackInProgress = false;
    player = new Player((GetScreenWidth() / 2) - 15, GetScreenHeight() * 0.75f);
    respawnCooldown = 1080;
    respawns = 0;
    count = 0;
    delay = 0;
    lives = 3;
    ResetScore();
}

//=========================== Phase 1.3 ===========================
void Program::SpawnEnemiesOnGameStart()
{
    Enemy::enemies.push_back(std::pair<std::pair<float, float>, Enemy *>{
        std::pair<float, float>{350, 150},
        new SpEnemy(350, 150)});

    Enemy::enemies.push_back(std::pair<std::pair<float, float>, Enemy *>{
        std::pair<float, float>{600, 150},
        new SpEnemy(600, 150)});

    for (int i = 0; i < 30; i++)
    {

        // x resett
        float x = 250 + 50 * (i % 10);

        // y increase
        float y = 200;

        if (i > 9 && i <= 19)
        {
            y += 50;
        }

        else if (i > 19)
        {
            y += 100;
        }

        Enemy::enemies.push_back(std::pair<std::pair<float, float>, Enemy *>{
            std::pair<float, float>{x, y},
            new StdEnemy(x, y)});
    }
}
//=========================== Phase 1.3 ===========================

void Program::UpdateScore(int points)
{
    score += points;
}

void Program::DrawScore(int score, float Ypos, float fontSize, Color color, Font font)
{
    // Measures the text size with the given font and spacing
    Vector2 textSize = MeasureTextEx(font, TextFormat("%i", score), fontSize, fontSize * .1f);

    // Centers the text horizontally on the screen
    Vector2 textPos = Vector2{
        // DO NOT TOUCH THE EXPRESSION ON THE X VALUE, IT JUST WORKS AND I HAVEN'T FIGURED OUT WHY
        Lerp(0.0f, (float)GetScreenWidth() - textSize.x, 1.0f * .5f), // Magic math for centering based on the size of the text
        Ypos};

    // Draws the text
    DrawTextEx(font, TextFormat("%i", score), textPos, fontSize, fontSize * .1f, color);
}

// In Program.cpp
void Program::DecreaseCooldownOnDifficulty(int score)
{
    difficulty newDifficulty;

    if (score >= HARD)
        newDifficulty = HARD;
    else if (score >= DIFFICULT)
        newDifficulty = DIFFICULT;
    else if (score >= TRICKY)
        newDifficulty = TRICKY;
    else
        newDifficulty = NORMAL;

    // Only update if difficulty changed
    if (newDifficulty != currentDifficulty)
    {
        currentDifficulty = newDifficulty;

        if (newDifficulty == TRICKY)
        {
            respawnCooldown = 1080 / 1.5f; // Use original value, not the current one
            std::cout << "Difficulty increased: TRICKY\n"
                      << "Respawn cooldown: " << respawnCooldown << std::endl;
        }
        else if (newDifficulty == DIFFICULT)
        {
            respawnCooldown = 1080 / 2.0f;
            std::cout << "Difficulty increased: DIFFICULT\n"
                      << "Respawn cooldown: " << respawnCooldown << std::endl;
        }
        else if (newDifficulty == HARD)
        {
            respawnCooldown = 1080 / 2.5f;
            std::cout << "Difficulty increased: HARD\n"
                      << "Respawn cooldown: " << respawnCooldown << std::endl;
        }
        else
        {
            respawnCooldown = 1080;
            std::cout << "Difficulty: NORMAL\n"
                      << "Respawn cooldown: " << respawnCooldown << std::endl;
        }
    }
}