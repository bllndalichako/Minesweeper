#include "Tile.h"

Tile::Tile() {
    backgroundTile.setTexture((TextureManager::GetTexture("tile_revealed")));
    revealedTile.setTexture(TextureManager::GetTexture("tile_hidden"));
    revealedImage = "tile_hidden";
    backgroundImage = "tile_revealed";
    isHidden = true;
    isFlagged = false;
    hasMine = false;
    hasWon = false;
    numAdjacentMines = 0;
    numTilesRevealed = 0;
}

void Tile::DrawTile(RenderWindow& window) {
    window.draw(backgroundTile);
    window.draw(revealedTile);
}

void Tile::SetHiddenState(bool hidden) {
    isHidden = hidden;
}

void Tile::SetPosition(Vector2f pos) {
    backgroundTile.setPosition(pos);
    revealedTile.setPosition(pos);
    position = pos;
}

void Tile::setTilesToPrint(string revealed, string background) {
    revealedTile.setTexture(TextureManager::GetTexture(revealed));
    revealedImage = revealed;
    backgroundTile.setTexture(TextureManager::GetTexture(background));
    backgroundImage = background;
}

bool Tile::Contains(Vector2i pos) {
    float xMin = position.x;
    float xMax = xMin + 32;
    float yMin = position.y;
    float yMax = yMin + 32;

    if (float(pos.x) > xMin && float(pos.x) < xMax) {
        if (float(pos.y) > yMin && float(pos.y) < yMax) {
            return true;
        }
    }
    return false;
}

bool Tile::HasFlag() {
    return isFlagged;
}

void Tile::SetHasFlag(bool hasFlag) {
    isFlagged = hasFlag;
}

bool Tile::HasHiddenTile() {
    return isHidden;
}

void Tile::PlayGame() {
    isHidden = false;
    ++numTilesRevealed;

    if (hasMine) {
        setTilesToPrint("mine", "tile_hidden");
        hasWon = false;
    }
    else {
        if (numAdjacentMines == 0) {
            setTilesToPrint("tile_revealed", "tile_revealed");
            RecursiveReveal();
        }
        else
            setTilesToPrint("number_" + to_string(numAdjacentMines), "tile_revealed");
    }
}

bool Tile::HasMine() {
    return hasMine;
}

void Tile::SetMine(bool hasMine) {
    this->hasMine = hasMine;
}

void Tile::SetAdjacentTiles(vector<Tile*>& tiles) {
    adjacentTiles = tiles;
    SetNumAdjacentMines();
}

void Tile::SetNumAdjacentMines() {
    for (auto & adjacentTile : adjacentTiles) {
        if (adjacentTile->HasMine()) {
            ++numAdjacentMines;
        }
    }
}

bool Tile::HasWon() {
    return hasWon;
}

void Tile::RecursiveReveal() {
    for (int i = 0; i < adjacentTiles.size(); ++i) {
        if (!adjacentTiles.at(i)->HasMine() && adjacentTiles.at(i)->HasHiddenTile() && !adjacentTiles.at(i)->HasFlag()) {
            ++numTilesRevealed;
            adjacentTiles.at(i)->SetHiddenState(false);

            if (adjacentTiles.at(i)->numAdjacentMines == 0) {
                adjacentTiles.at(i)->setTilesToPrint("tile_revealed", "tile_revealed");
                adjacentTiles.at(i)->RecursiveReveal();
            }
            else
                adjacentTiles.at(i)->setTilesToPrint("number_" + to_string(adjacentTiles.at(i)->numAdjacentMines), "tile_revealed");
        }
    }
}

string Tile::getRevealedImage() {
    return revealedImage;
}

string Tile::getBackgroundImage() {
    return backgroundImage;
}

int Tile::getNumTilesRevealed() {
    return numTilesRevealed;
}
