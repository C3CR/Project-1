#ifndef CARD_H
#define CARD_H

#include <iostream>
#include <string>

using namespace std;

struct Card
{
    int rank;
    int suit;

    Card() : rank(0), suit(0) {}

    Card(int r, int s) : rank(r), suit(s) {}

    string getRankString() const
    {
        switch (rank)
        {
            case 1: return "Ace";
            case 2: return "2";
            case 3: return "3";
            case 4: return "4";
            case 5: return "5";
            case 6: return "6";
            case 7: return "7";
            case 8: return "8";
            case 9: return "9";
            case 10: return "10";
            case 11: return "Jack";
            case 12: return "Queen";
            case 13: return "King";
            default: return "Unknown";
        }
    }

    string getSuitString() const
    {
        switch (suit)
        {
            case 0: return "Hearts";
            case 1: return "Diamonds";
            case 2: return "Clubs";
            case 3: return "Spades";
            default: return "Unknown";
        }
    }

    friend ostream& operator<<(ostream& os, const Card& card)
    {
        os << card.getRankString() << " of " << card.getSuitString();
        return os;
    }

    bool operator==(const Card& other) const
    {
        return rank == other.rank && suit == other.suit;
    }
};

#endif
