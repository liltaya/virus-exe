#include "card.hpp"

// default constructor
Card::Card() : suit(0), rank(1) {
}

// constructor to set the suit and rank of a card
Card::Card(int s, int r) : suit(s), rank(r) {
}

// returns the suit of the card
int Card::getSuit() const {
    return suit;
}

// returns the rank of the card
int Card::getRank() const {
    return rank;
}

// returns the amt of damage or healing based on the rank
int Card::getEffectValue() const {
    // num cards deal damage equal to their rank
    if (rank >= 2 && rank <= 10) {
        return rank;
    }
    else if (rank == 11) {
        return 2; // jack heals 2 HP
    }
    else if (rank == 12) {
        return 4; // queen heals 4 HP
    }
    else if (rank == 13) {
        return 6; // king heals 6 HP
    }

    return 0; // ace shields instead of changing HP
}

// returns the card's rank and suit in a readable way
string Card::getDescription() const {
    string rankName;
    string suitName;

    // convert rank to its name
    if (rank == 1) {
        rankName = "Ace";
    }
    else if (rank == 11) {
        rankName = "Jack";
    }
    else if (rank == 12) {
        rankName = "Queen";
    }
    else if (rank == 13) {
        rankName = "King";
    }
    else {
        rankName = to_string(rank);
    }

    // convert suit number to its name
    if (suit == 0) {
        suitName = "Hearts";
    }
    else if (suit == 1) {
        suitName = "Diamonds";
    }
    else if (suit == 2) {
        suitName = "Clubs";
    }
    else if (suit == 3) {
        suitName = "Spades";
    }
    else {
        suitName = "Unknown";
    }

    return rankName + " of " + suitName; // ex. jack of hearts
}
