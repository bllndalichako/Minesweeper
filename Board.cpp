#include "Board.h"
#include <ctime>

Board::Board() = default;

Board::Board(int numColumns, int numRows, int mineCount) {
    vector<vector<Tile>> tiles(numRows, vector<Tile>(numColumns));
    theBoard = tiles;
    confgNumMines = mineCount;
    numMines = confgNumMines;
    width = (float)numColumns * 32;
    height = (float)numRows * 32;
    happySmiley = true;
    winSmiley = false;
    loseSmiley = false;
    debugModeOn = false;
    numFlags = 0;
    SetMines();
    AssociateTiles();
}

void Board::revealTile(Vector2i mousePos) {
    for (int i = 0; i < theBoard.size(); ++i) {
        for (int j = 0; j < theBoard.at(i).size(); ++j) {
            if (theBoard[i][j].Contains(mousePos) && theBoard[i][j].HasHiddenTile() && !theBoard[i][j].HasFlag()) {
                theBoard[i][j].SetHiddenState(false);
                theBoard[i][j].PlayGame();
                GameOutcome(theBoard[i][j]);
            }
        }
    }
}

void Board::ToggleFlag(Vector2i mousePos) { //TODO Flags go on the mines when using the test boards
    for (int i = 0; i < theBoard.size(); ++i) {
        for (int j = 0; j < theBoard.at(i).size(); ++j) {
            if (theBoard[i][j].Contains(mousePos) && theBoard[i][j].HasFlag()) {
                --numFlags;
                theBoard[i][j].SetHasFlag(false);
                if (debugModeOn && theBoard[i][j].getRevealedImage() == "mine")
                    theBoard[i][j].setTilesToPrint("mine", "tile_hidden");
                else
                    theBoard[i][j].setTilesToPrint("tile_hidden", "tile_revealed");
                break;
            }
            else if (theBoard[i][j].Contains(mousePos) && !theBoard[i][j].HasFlag() && theBoard[i][j].HasHiddenTile()) {
                ++numFlags;
                theBoard[i][j].SetHasFlag(true);
                if (debugModeOn && theBoard[i][j].getRevealedImage() == "mine")
                    theBoard[i][j].setTilesToPrint("mine", "flag");
                else
                    theBoard[i][j].setTilesToPrint("flag", "tile_hidden");
                break;
            }
        }
    }
}

void Board::debugButtonFunct() {
    for (int i = 0; i < theBoard.size(); ++i) {
        for (int j = 0; j < theBoard.at(i).size(); ++j) {
            if (theBoard[i][j].HasMine() && !debugModeOn) {
                theBoard[i][j].setTilesToPrint("mine", theBoard[i][j].getRevealedImage());
            }
            else if (theBoard[i][j].HasMine() && debugModeOn && theBoard[i][j].HasHiddenTile()) {
                theBoard[i][j].setTilesToPrint(theBoard[i][j].getBackgroundImage(), "tile_hidden");
            }
        }
    }
    debugModeOn = !debugModeOn;
}

void Board::smileyButtonFunct() {
    vector<vector<Tile>> tiles(theBoard.size(), vector<Tile>(theBoard[0].size()));
    theBoard = tiles;
    happySmiley = true;
    winSmiley = false;
    loseSmiley = false;
    debugModeOn = false;
    numMines = confgNumMines;
    numFlags = 0;

    SetMines();
    AssociateTiles();
}

void Board::SetMines() {
    int row, column;
    unsigned int mineCount = 0;

    // Resetting any existing mines to none
    for (int i = 0; i < theBoard.size(); ++i) {
        for (int j = 0; j < theBoard[i].size(); ++j) {
            theBoard[i][j].SetMine(false);
        }
    }

    while (mineCount != numMines) {
        row = Random::RandomInt(0, theBoard.size() - 1);
        column = Random::RandomInt(0, theBoard[0].size() - 1);

        if (!theBoard[row][column].HasMine()) {
            theBoard[row][column].SetMine(true);
            ++mineCount;
        }
    }
}

void Board::test1ButtonFunct() {
    setTestBoard("boards/testboard1.brd");
}

void Board::test2ButtonFunct() {
    setTestBoard("boards/testboard2.brd");
}

void Board::test3ButtonFunct() {
    setTestBoard("boards/testboard3.brd");
}

void Board::setTestBoard(string filePath) { // FIXME Should the debug button work with the test boards?
    happySmiley = true;
    winSmiley = false;
    loseSmiley = false;
    debugModeOn = false;
    numFlags = 0;
    numMines = 0;
    vector<string> testTiles;

    ifstream testBoardFile;
    testBoardFile.open(filePath);
    vector<vector<Tile>> testBoard(theBoard.size(), vector<Tile>(theBoard[0].size()));

    if (testBoardFile.is_open()) {
        string rowLayout;

        while (testBoardFile.good()) {
            getline(testBoardFile, rowLayout);
            testTiles.push_back(rowLayout);
        }
        testBoardFile.close();
    }

    for (int i = 0; i < testTiles.size(); ++i) {
        for (int j = 0; j < testTiles[i].size(); ++j) {
            if (testTiles[i].substr(j, 1) == "1") {
                testBoard[i][j].SetMine(true);
                ++numMines;
            }
            else if (testTiles[i].substr(j, 1) == "0") {
                testBoard[i][j].SetMine(false);
            }
        }
    }
    theBoard = testBoard;
    AssociateTiles();
}

void Board::AssociateTiles() {
    int iRowAbove, iRowBelow, jLeftColumn, jRightColumn;

    for (int i = 0; i < theBoard.size(); ++i) {
        for (int j = 0; j < theBoard.at(i).size(); ++j) {
            vector<Tile*> adjacentTiles;
            iRowAbove = i - 1;
            iRowBelow = i + 1;
            jLeftColumn = j - 1;
            jRightColumn = j + 1;

            // Check if has row above it
            if (iRowAbove >= 0) {
                //Add the tile right above it i.e same column (1)
                adjacentTiles.push_back(&theBoard[iRowAbove][j]);
                //Check if has left column
                if (jLeftColumn >= 0) {
                    //Add the tiles on this column X row above (1)
                    adjacentTiles.push_back(&theBoard[iRowAbove][jLeftColumn]);
                }
                //Check if it has right column 2
                if (jRightColumn < theBoard[i].size()) {
                    //Add the tiles on this column X row above (1)
                    adjacentTiles.push_back(&theBoard[iRowAbove][jRightColumn]);
                }
            }
            // Check if it has a row below it
            if (iRowBelow < theBoard.size()) {
                //Add the tile right below it i.e same column (1)
                adjacentTiles.push_back(&theBoard[iRowBelow][j]);

                //Check if has left column
                if (jLeftColumn >= 0) {
                    //Add the tile on this column X the row below (1)
                    adjacentTiles.push_back(&theBoard[iRowBelow][jLeftColumn]);
                }

                //Check if it has right column
                if (jRightColumn < theBoard[i].size()) {
                    //Add the tile on this column X the row below (1)
                    adjacentTiles.push_back(&theBoard[iRowBelow][jRightColumn]);
                }
            }

            //Adding tiles from the same row but different columns (2)
            if (jLeftColumn >= 0)
                adjacentTiles.push_back(&theBoard[i][jLeftColumn]);

            if (jRightColumn < theBoard[i].size())
                adjacentTiles.push_back(&theBoard[i][jRightColumn]);

            theBoard[i][j].SetAdjacentTiles(adjacentTiles);
        }
    }
}

void Board::GameOutcome(Tile& currentTile) {
    int numReavealedTiles = 0;

    for (int i = 0; i < theBoard.size(); ++i) {
        for (int j = 0; j < theBoard.at(i).size(); ++j) {
            if (!theBoard[i][j].HasHiddenTile())
                ++numReavealedTiles;
        }
    }

    if (currentTile.HasMine()) {
        loseSmiley = true;
        happySmiley = false;
        winSmiley = false;

        for (int i = 0; i < theBoard.size(); ++i) {
            for (int j = 0; j < theBoard.at(i).size(); ++j) {
                if (theBoard[i][j].HasMine()) {
                    if (theBoard[i][j].HasFlag())
                        theBoard[i][j].setTilesToPrint("mine", "flag");
                    else 
                        theBoard[i][j].setTilesToPrint("mine", "tile_revealed");
                }
            }
        }
    }
    else if (numReavealedTiles == theBoard.size() * theBoard[0].size() - numMines) {
        winSmiley = true;
        happySmiley = false;
        loseSmiley = false;

        for (int i = 0; i < theBoard.size(); ++i) {
            for (int j = 0; j < theBoard.at(i).size(); ++j) {
                if (theBoard[i][j].HasMine()) {
                    theBoard[i][j].setTilesToPrint("flag", "tile_hidden");
                    theBoard[i][j].SetHasFlag(true);
                    ++numFlags;
                }
            }
        }
    }
    else {
        happySmiley = true;
        winSmiley = false;
        loseSmiley = false;
    }
}

void Board::initializeButtons() {
    digitsPanel.setTexture(TextureManager::GetTexture("digits"));
    debugButton.setTexture(TextureManager::GetTexture("debug"));
    test1Button.setTexture(TextureManager::GetTexture("test_1"));
    test2Button.setTexture(TextureManager::GetTexture("test_2"));
    test3Button.setTexture(TextureManager::GetTexture("test_3"));

    if (happySmiley)
        smileyButton.setTexture(TextureManager::GetTexture("face_happy"));
    else if (winSmiley)
        smileyButton.setTexture(TextureManager::GetTexture("face_win"));
    else if (loseSmiley)
        smileyButton.setTexture(TextureManager::GetTexture("face_lose"));
}

void Board::DrawButtons(RenderWindow& window) {
    test3Button.setPosition({width - 64.0f, height});
    window.draw(test3Button);

    test2Button.setPosition({width - 2 * 64.0f, height});
    window.draw(test2Button);

    test1Button.setPosition({width - 3 * 64.0f, height});
    window.draw(test1Button);

    debugButton.setPosition({width - 4 * 64.0f, height});
    window.draw(debugButton);

    smileyButton.setPosition({width - 6 * 64.0f, height});
    window.draw(smileyButton);

    string digitsDisplay = to_string(numMines - numFlags);

    for (int i = 0; i < digitsDisplay.size(); ++i) {
        if (digitsDisplay.substr(i, 1) == "-") {
            IntRect digitRect(21 * 10, 0, 21, 32);
            digitsPanel.setTextureRect(digitRect);
        }
        else {
            IntRect digitRect(21 * stoi(digitsDisplay.substr(i, 1)), 0, 21, 32);
            digitsPanel.setTextureRect(digitRect);
        }
        digitsPanel.setPosition({21.0f * i, height});
        window.draw(digitsPanel);
    }
}

void Board::DrawBoard(RenderWindow &window) {
    initializeButtons();
    Sprite background(TextureManager::GetTexture("tile_hidden"));
    for (int i = 0; i < theBoard.size(); ++i) {
        for (int j = 0; j < theBoard.at(i).size(); ++j) {
            background.setPosition({j * 32.0f, i * 32.0f});
            window.draw(background);
            theBoard[i][j].SetPosition({j * 32.0f, i * 32.0f});
            theBoard[i][j].DrawTile(window);
        }
    }
    DrawButtons(window);
}

Sprite* Board::getSmileyButton() {
    return &smileyButton;
}

Sprite *Board::getDebugButton() {
    return &debugButton;
}

Sprite *Board::getTest1Button() {
    return &test1Button;
}

Sprite *Board::getTest2Button() {
    return &test2Button;
}

Sprite *Board::getTest3Button() {
    return &test3Button;
}

bool Board::smileyIsHappy() {
    return happySmiley;
}