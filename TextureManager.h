#pragma once
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <string>
using namespace std;
using namespace sf;

class TextureManager {
    static unordered_map<string, Texture> textures;
    static void LoadTexture(string fileName);

public:
    static Texture& GetTexture(string textureName);
    static void Clear(); // Call once at the end of main
};