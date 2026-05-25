/*
*   Game.h
*   Cesar CSC17C
*   Project 1 Go Fish
*   
*   Game.h contains the Game class which helps manage 
*   the game flow like: dealing cards, taking turns, 
*   and checking conditions.
*   
*   
*   
*   
*   
*   
*   
*   
*   
*   
*   
*/

#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>
#include <list>
#include <map>
#include <algorithm>
#include <utility>
#include <cstdlib>
#include "Card.h"
#include "Deck.h"
#include "Player.h"

using namespace std;

class Game
{
private:
    list<Player> players;
    Deck deck;
    int numPlayers;
    bool gameOver;

public:
    Game() : numPlayers(0), gameOver(false) {}

    void setup()
    {
        cout << "Welcome to Go Fish!" << endl;

        cout << "How many players? Enter 2, 3, or 4: ";
        cin >> numPlayers;

        while (numPlayers < 2 || numPlayers > 4)
        {
            cout << "Please enter 2, 3, or 4: ";
            cin >> numPlayers;
        }

        for (int i = 1; i <= numPlayers; i++)
        {
            string playerName;

            if (i == 1)
            {
                cout << "Enter your name: ";
                cin >> playerName;
            }
            else
            {
                playerName = "Computer " + to_string(i - 1);
                cout << "Adding " << playerName << " to the game." << endl;
            }

            players.push_back(Player(playerName));
        }

        cout << endl;
        cout << numPlayers << " players have been added to the game." << endl;
    }

    void dealCards()
    {
        cout << "Dealing cards..." << endl;

        int cardsPerPlayer;

        if (numPlayers == 2)
        {
            cardsPerPlayer = 7;
        }
        else
        {
            cardsPerPlayer = 5;
        }

        for (int round = 0; round < cardsPerPlayer; round++)
        {
            for (list<Player>::iterator it = players.begin(); it != players.end(); ++it)
            {
                if (!deck.isEmpty())
                {
                    Card card = deck.drawCard();
                    it->addCard(card);
                    it->checkForBooks();
                }
            }
        }

        cout << endl;
        cout << "Cards have been dealt. Let's start the game!" << endl;
    }

    void displayScores() const
    {
        cout << endl;
        cout << "Current Scores:" << endl;

        for (list<Player>::const_iterator it = players.begin(); it != players.end(); ++it)
        {
            cout << " " << it->getName() << ": " << it->getScore() << " books" << endl;
            it->printBooks();
        }

        cout << endl;
    }

    bool isGameOver()
    {
        int totalBooks = 0;

        for (list<Player>::iterator it = players.begin(); it != players.end(); ++it)
        {
            totalBooks += it->getScore();
        }

        if (totalBooks >= 13)
        {
            return true;
        }

        if (deck.isEmpty())
        {
            for (list<Player>::iterator it = players.begin(); it != players.end(); ++it)
            {
                if (!it->hasCards())
                {
                    return true;
                }
            }
        }

        return false;
    }

    void humanTurn(Player& human)
    {
        cout << endl;
        cout << human.getName() << ", it is your turn!" << endl;

        if (!human.hasCards())
        {
            cout << "You have no cards. Drawing from deck..." << endl;

            if (!deck.isEmpty())
            {
                human.addCard(deck.drawCard());
                human.checkForBooks();
            }
            else
            {
                cout << "The deck is empty. You cannot draw." << endl;
                return;
            }
        }

        human.printHand();

        cout << endl;
        cout << "Players you can ask:" << endl;

        list<Player*> opponents;
        int index = 1;

        for (list<Player>::iterator it = players.begin(); it != players.end(); ++it)
        {
            if (&(*it) != &human)
            {
                cout << " " << index << ". " << it->getName()
                     << " (" << it->handSize() << " cards)" << endl;

                opponents.push_back(&(*it));
                index++;
            }
        }

        cout << "Pick a player to ask 1-" << opponents.size() << ": ";

        int playerChoice;
        cin >> playerChoice;

        while (playerChoice < 1 || playerChoice > static_cast<int>(opponents.size()))
        {
            cout << "Invalid choice. Try again: ";
            cin >> playerChoice;
        }

        list<Player*>::iterator oppIt = opponents.begin();

        for (int i = 1; i < playerChoice; i++)
        {
            ++oppIt;
        }

        Player* target = *oppIt;

        int rankToAsk = human.chooseRankToAsk(true);

        int received = human.askForCards(*target, rankToAsk);

        if (received == 0)
        {
            cout << "Go Fish! Drawing a card from the deck..." << endl;

            if (!deck.isEmpty())
            {
                Card drawn = deck.drawCard();
                human.addCard(drawn);
                human.checkForBooks();

                if (drawn.rank == rankToAsk)
                {
                    cout << "You drew what you asked for! Take another turn!" << endl;
                    humanTurn(human);
                }
            }
            else
            {
                cout << "Deck is empty! No card to draw." << endl;
            }
        }
        else
        {
            human.checkForBooks();
        }

        cout << "Deck has " << deck.cardsLeft() << " cards remaining." << endl;
    }

    void computerTurn(Player& computer)
    {
        cout << endl;
        cout << computer.getName() << "'s turn." << endl;

        if (!computer.hasCards())
        {
            cout << computer.getName() << " has an empty hand, drawing..." << endl;

            if (!deck.isEmpty())
            {
                computer.addCard(deck.drawCard());
                computer.checkForBooks();
            }
            else
            {
                cout << "Deck is empty! No card to draw." << endl;
                return;
            }
        }

        list<Player*> opponents;

        for (list<Player>::iterator it = players.begin(); it != players.end(); ++it)
        {
            if (&(*it) != &computer)
            {
                opponents.push_back(&(*it));
            }
        }

        if (opponents.empty())
        {
            return;
        }

        int randomIndex = rand() % opponents.size();

        list<Player*>::iterator oppIt = opponents.begin();

        for (int i = 0; i < randomIndex; i++)
        {
            ++oppIt;
        }

        Player* target = *oppIt;

        int rankToAsk = computer.chooseRankToAsk(false);

        if (rankToAsk == -1)
        {
            return;
        }

        int received = computer.askForCards(*target, rankToAsk);

        if (received == 0)
        {
            if (!deck.isEmpty())
            {
                Card drawn = deck.drawCard();
                computer.addCard(drawn);
                computer.checkForBooks();

                cout << computer.getName() << " goes fishing and draws a card." << endl;

                if (drawn.rank == rankToAsk)
                {
                    cout << computer.getName()
                         << " drew what they asked for and gets another turn!" << endl;

                    computerTurn(computer);
                }
            }
        }
        else
        {
            computer.checkForBooks();
        }

        cout << "Deck has " << deck.cardsLeft() << " cards remaining." << endl;
    }

    void playGame()
    {
        setup();
        dealCards();

        cout << endl;
        cout << "Let's play Go Fish!" << endl;

        while (!isGameOver())
        {
            displayScores();

            for (list<Player>::iterator it = players.begin();
                 it != players.end() && !isGameOver();
                 ++it)
            {
                if (it == players.begin())
                {
                    humanTurn(*it);
                }
                else
                {
                    computerTurn(*it);

                    cout << "[Press Enter to continue...]" << endl;
                    cin.ignore();
                    cin.get();
                }
            }
        }

        displayFinalResults();
    }

    void displayFinalResults()
    {
        map<string, int> scoreboard;

        cout << endl;
        cout << "Game Over!" << endl;

        for (list<Player>::iterator it = players.begin(); it != players.end(); ++it)
        {
            scoreboard[it->getName()] = it->getScore();
        }

        cout << endl;
        cout << "Final Score:" << endl;

        for (map<string, int>::const_iterator it = scoreboard.begin();
             it != scoreboard.end();
             ++it)
        {
            cout << " " << it->first << ": " << it->second << " book(s)" << endl;
        }

        map<string, int>::const_iterator winner = scoreboard.begin();

        for (map<string, int>::const_iterator it = scoreboard.begin();
             it != scoreboard.end();
             ++it)
        {
            if (it->second > winner->second)
            {
                winner = it;
            }
        }

        cout << endl;
        cout << "The Winner: " << winner->first
             << " with " << winner->second << " book(s)!" << endl;

        cout << endl;
        cout << "Final Book Details:" << endl;

        for (list<Player>::iterator it = players.begin(); it != players.end(); ++it)
        {
            it->printBooks();
            it->printDiscardPile();
            cout << endl;
        }
    }
};

#endif
