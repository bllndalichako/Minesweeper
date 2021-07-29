#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "Board.h"
using namespace std;
using namespace sf;

int main() {
    int numColumns = 0;
    int numRows = 0;
    int numMines = 0;

    ifstream configFile;
    configFile.open("boards/config.cfg");

    if (configFile.is_open()) {
        string columns, rows, mines;

        while (configFile.good()) {
            getline(configFile, columns);
            stringstream columnConvrt(columns);
            columnConvrt >> numColumns;

            getline(configFile, rows);
            stringstream rowConvrt(rows);
            rowConvrt >> numRows;

            getline(configFile, mines);
            stringstream minesConvrt(mines);
            minesConvrt >> numMines;
        }
        configFile.close();
    }

    RenderWindow window(VideoMode(numColumns * 32, numRows * 32 + 88), "Minesweeper");
    Board newBoard(numColumns, numRows, numMines);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    auto mousePos = Mouse::getPosition(window);

                    if (newBoard.getSmileyButton()->getGlobalBounds().contains(mousePos.x, mousePos.y))
                        newBoard.smileyButtonFunct();
                    else if (newBoard.getDebugButton()->getGlobalBounds().contains(mousePos.x, mousePos.y) && newBoard.smileyIsHappy())
                        newBoard.debugButtonFunct();
                    else if (newBoard.getTest1Button()->getGlobalBounds().contains(mousePos.x, mousePos.y))
                        newBoard.test1ButtonFunct();
                    else if (newBoard.getTest2Button()->getGlobalBounds().contains(mousePos.x, mousePos.y))
                        newBoard.test2ButtonFunct();
                    else if (newBoard.getTest3Button()->getGlobalBounds().contains(mousePos.x, mousePos.y))
                        newBoard.test3ButtonFunct();
                    else if (newBoard.smileyIsHappy())
                        newBoard.revealTile(mousePos);
                }
                
                if (event.mouseButton.button == Mouse::Right && newBoard.smileyIsHappy())
                    newBoard.ToggleFlag(Mouse::getPosition(window));
            }
        }
        window.clear();
        newBoard.DrawBoard(window);
        window.display();
    }
    TextureManager::Clear();
    return 0;
}