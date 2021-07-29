#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <random>
#include <vector>
#include "Tile.h"
#include "TextureManager.h"
#include "Random.h"
using namespace std;
using namespace sf;

class Board {
    Sprite smileyButton, digitsPanel, debugButton, test1Button, test2Button, test3Button;
    vector<vector<Tile>> theBoard;
    int numMines, numFlags, confgNumMines;
    float width, height;
    bool happySmiley, winSmiley, loseSmiley, debugModeOn;


public:
    // CONSTRUCTORS
    Board();
    Board(int numColumns, int numRows, int numMines);

    // FUNCTIONALITY
    void revealTile(Vector2i mousePos);
    void ToggleFlag(Vector2i mousePos);
    void debugButtonFunct();
    void smileyButtonFunct();
    void SetMines();
    void test1ButtonFunct();
    void test2ButtonFunct();
    void test3ButtonFunct();
    void setTestBoard(string filePath);
    void AssociateTiles();
    void GameOutcome(Tile& currentTile);
    void initializeButtons();
    void DrawButtons(RenderWindow& window);
    void DrawBoard(RenderWindow &window);

    // ACCESSORS
    Sprite* getSmileyButton();
    Sprite* getDebugButton();
    Sprite* getTest1Button();
    Sprite* getTest2Button();
    Sprite* getTest3Button();
    bool smileyIsHappy();
};