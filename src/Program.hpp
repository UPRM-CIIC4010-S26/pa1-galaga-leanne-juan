#pragma once

#include <iostream>
#include <string>
#include "raylib-cpp/raylib-cpp.hpp"
#include "Background.hpp"
#include "Player.hpp"
#include "StdEnemy.hpp"
#include "SpEnemy.hpp"
#include "StEnemy.hpp"
#include "DyEnemy.hpp"
class Program
{
private:
    Background background = Background();
    Player *player = new Player((GetScreenWidth() / 2) - 15, GetScreenHeight() * 0.75f);
    int respawnCooldown = 1080;
    int respawns = 0;
    int count = 0;
    int delay = 0;
    int lives = 3;
    int pauseFrames = 0;

    bool startup = true;
    bool paused = false;
    bool gameOver = false;

public:
    int score = 0;

    Program();

    void Update();
    void Draw();
    void ManageEnemyRespawns();
    void DrawStartup();
    void DrawPauseScreen();
    void DrawGameOver();
    void KeyInputs();
    void PlayerReset();
    void Reset();

    //=========================== Phase 1.3 ===========================
    void SpawnEnemiesOnGameStart();
    //=========================== Phase 1.3 ===========================

    // score
    void UpdateScore(int points);
    void ResetScore() { score = 0; }
    void DrawScore(int score, float Ypos, float fontSize, Color color, Font font = GetFontDefault());

    ~Program() {}
};