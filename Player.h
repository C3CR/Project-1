#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>
#include <list>
#include <map>
#include <set>
#include <stack>
#include <algorithm>
#include "Card.h"

using namespace std;

class Player
{
private:
    string name;
    list<Card> hand;
    map<int, int> rankCount;
    set<int> completedBooks;
    stack<Card> discardPile;
    int score;

public:
    Player() : name("Player"), score(0) {}

    Player(string playerName) : name(playerName), score(0) {}

    string getName() const
    {
        return name;
    }

    int getScore() const
    {
        return score;
    }

    int handSize() const
    {
        return static_cast<int>(hand.size());
    }

    bool hasCards() const
    {
        return !hand.empty();
    }

    void addCard(const Card& c)
    {
        hand.push_back(c);
        rankCount[c.rank]++;

        cout << " >> " << name << " draws: " << c << endl;
    }

    bool hasRank(int rank) const
    {
        map<int, int>::const_iterator it = rankCount.find(rank);

        if (it != rankCount.end() && it->second > 0)
        {
            return true;
        }

        return false;
    }

    list<Card> giveCardsOfRank(int rank)
    {
        list<Card> givenCards;
        list<Card>::iterator it = hand.begin();

        while (it != hand.end())
        {
            if (it->rank == rank)
            {
                givenCards.push_back(*it);
                it = hand.erase(it);
            }
            else
            {
                ++it;
            }
        }

        if (!givenCards.empty())
        {
            rankCount[rank] = 0;

            cout << " >> " << name << " gives " << givenCards.size()
                 << " card(s) of rank " << Card(rank, 0).getRankString() << "." << endl;
        }

        return givenCards;
    }

    void receiveCards(const list<Card>& cards)
    {
        for (list<Card>::const_iterator it = cards.begin(); it != cards.end(); ++it)
        {
            hand.push_back(*it);
            rankCount[it->rank]++;
        }
    }

    void checkForBooks()
    {
        list<int> booksToRemove;

        for (map<int, int>::iterator it = rankCount.begin(); it != rankCount.end(); ++it)
        {
            int rank = it->first;
            int count = it->second;

            if (completedBooks.count(rank) > 0)
            {
                continue;
            }

            if (count >= 4)
            {
                booksToRemove.push_back(rank);
            }
        }

        for (list<int>::iterator it = booksToRemove.begin(); it != booksToRemove.end(); ++it)
        {
            int rank = *it;

            cout << "\n*** " << name << " completed a BOOK of "
                 << Card(rank, 0).getRankString() << "s! ***\n\n";

            list<Card> bookCards = giveCardsOfRank(rank);

            for (list<Card>::iterator cardIt = bookCards.begin(); cardIt != bookCards.end(); ++cardIt)
            {
                discardPile.push(*cardIt);
            }

            completedBooks.insert(rank);
            score++;
        }
    }

    int askForCards(Player& other, int rank)
    {
        cout << name << " asks " << other.getName()
             << ": Do you have any " << Card(rank, 0).getRankString() << "s?" << endl;

        if (other.hasRank(rank))
        {
            list<Card> received = other.giveCardsOfRank(rank);
            receiveCards(received);

            cout << other.getName() << " says: Here you go! Take "
                 << received.size() << " card(s)." << endl;

            checkForBooks();

            return static_cast<int>(received.size());
        }
        else
        {
            cout << other.getName() << " says: Go Fish!" << endl;
            return 0;
        }
    }

    int chooseRankToAsk(bool isHuman)
    {
        if (hand.empty())
        {
            return -1;
        }

        if (isHuman)
        {
            printHand();

            cout << "Enter the rank number you want to ask for ";
            cout << "(1=Ace, 11=Jack, 12=Queen, 13=King): ";

            int choice;
            cin >> choice;

            while (!hasRank(choice))
            {
                cout << "[You do not have that rank in your hand. Pick again]: ";
                cin >> choice;
            }

            return choice;
        }
        else
        {
            int bestRank = hand.front().rank;
            int bestCount = 0;

            for (map<int, int>::const_iterator it = rankCount.begin(); it != rankCount.end(); ++it)
            {
                if (it->second > bestCount && completedBooks.count(it->first) == 0)
                {
                    bestCount = it->second;
                    bestRank = it->first;
                }
            }

            return bestRank;
        }
    }

    void printHand() const
    {
        cout << "\n--- " << name << "'s Hand (" << hand.size() << " cards) ---\n";

        int i = 1;

        for (list<Card>::const_iterator it = hand.begin(); it != hand.end(); ++it)
        {
            cout << " " << i << ". " << *it << endl;
            i++;
        }

        cout << "[Rank counts: ";

        for (map<int, int>::const_iterator it = rankCount.begin(); it != rankCount.end(); ++it)
        {
            if (it->second > 0)
            {
                cout << Card(it->first, 0).getRankString() << ":" << it->second << " ";
            }
        }

        cout << "]" << endl;
    }

    void printBooks() const
    {
        cout << name << "'s completed books (" << completedBooks.size() << "): ";

        for (set<int>::const_iterator it = completedBooks.begin(); it != completedBooks.end(); ++it)
        {
            cout << Card(*it, 0).getRankString() << " ";
        }

        cout << endl;
    }

    void printDiscardPile() const
    {
        cout << name << "'s discard pile/books: " << endl;

        stack<Card> copy = discardPile;

        while (!copy.empty())
        {
            cout << " - " << copy.top() << endl;
            copy.pop();
        }
    }

    int countCardsOfRank(int rank) const
    {
        map<int, int>::const_iterator it = rankCount.find(rank);

        if (it != rankCount.end())
        {
            return it->second;
        }

        return 0;
    }

    const list<Card>& getHand() const
    {
        return hand;
    }

    const set<int>& getBooks() const
    {
        return completedBooks;
    }
};

#endif
