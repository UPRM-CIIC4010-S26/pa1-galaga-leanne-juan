#pragma once
#include "raylib-cpp/raylib-cpp.hpp"

class ImageManager {
    public:
        inline static Texture2D SpriteSheet;

        // Add heart icon for lives 
        inline static Texture2D Heart;

        static void Load() {
            SpriteSheet = LoadTextureFromImage(LoadImage("images/Arcade - Galaga - Miscellaneous - General Sprites.png"));

            // Load heart icon
            Heart = LoadTextureFromImage(LoadImage("images/pixil-frame-0_15.png"));
        }

        static void Unload() {
            UnloadTexture(SpriteSheet);

            // Unload heart icon
            UnloadTexture(Heart);
        }
};