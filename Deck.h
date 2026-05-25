#ifndef DECK_H
#define DECK_H

#include <iostream>
#include <queue>
#include <list>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "Card.h"

using namespace std;

class Deck
{
private:
    queue<Card> drawPile;

public:
    Deck()
    {
        buildAndShuffle();
    }

    void buildAndShuffle()
    {
        queue<Card> emptyQueue;
        swap(drawPile, emptyQueue);

        list<Card> tempList;

        for (int suit = 0; suit < 4; suit++)
        {
            for (int rank = 1; rank <= 13; rank++)
            {
                tempList.push_back(Card(rank, suit));
            }
        }

        Card deckArray[52];
        int index = 0;

        for (list<Card>::iterator it = tempList.begin(); it != tempList.end(); ++it)
        {
            deckArray[index] = *it;
            index++;
        }

        random_shuffle(deckArray, deckArray + 52);

        for (int i = 0; i < 52; i++)
        {
            drawPile.push(deckArray[i]);
        }

        cout << "[Deck built and shuffled. " << drawPile.size() << " cards ready.]" << endl;
    }

    Card drawCard()
    {
        if (drawPile.empty())
        {
            cout << "Deck is empty! Cannot draw." << endl;
            return Card(0, 0);
        }

        Card topCard = drawPile.front();
        drawPile.pop();

        return topCard;
    }

    int cardsLeft() const
    {
        return static_cast<int>(drawPile.size());
    }

    bool isEmpty() const
    {
        return drawPile.empty();
    }
};

#endif
