/*
*   Author: Cesar Cervantes
*   Class: CSC17C
*   Project1: Go Fish Card Game
*
*   Files: - main.cpp
*          - Card.h
*          - Deck.h
*          - Player.h
*          - Game.h
*          - write.txt
*
*
*
*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Game.h"

using namespace std;

int main()
{
    srand(static_cast<unsigned int>(time(0)));

    Game myGame;
    myGame.playGame();

    cout << "Thanks for playing Go Fish! (by Cesar)" << endl;

    return 0;
}
