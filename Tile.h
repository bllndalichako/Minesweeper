#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "TextureManager.h"
#include "Random.h"
using namespace std;
using namespace sf;

class Tile {
    private:
        Sprite revealedTile, backgroundTile;
        string revealedImage, backgroundImage;
        bool isHidden, isFlagged, hasMine, hasWon;
        Vector2f position;
        vector<Tile*> adjacentTiles;
        int numAdjacentMines, numTilesRevealed;

    public:
        // CONSTRUCTORS
        Tile();

        // MUTATORS
        void SetHiddenState(bool hidden);
        void setTilesToPrint(string revealed, string background);
        void SetPosition(Vector2f pos);
        void SetHasFlag(bool hasFlag);
        void SetMine(bool hasMine);
        void SetAdjacentTiles(vector<Tile*>& tiles);
        void SetNumAdjacentMines();

        // ACCESSORS
        bool HasFlag();
        bool HasHiddenTile();
        bool HasMine();
        bool HasWon();
        string getRevealedImage();
        string getBackgroundImage();
        int getNumTilesRevealed();

        // FUNCTIONALITY
        bool Contains(Vector2i pos);
        void PlayGame(); 
        void RecursiveReveal();
        void DrawTile(RenderWindow& window);
};