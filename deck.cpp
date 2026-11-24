#include "deck.hpp"
#include "player.hpp" // so we can directly add card to players hand when card drawn from deck
#include <cstdlib> // need rand() and srand(), for shuffling
#include <ctime> // time for random seeding, for shuffling

Deck::Deck() : deckCount(52), discardedCount(0) { // creates a deck of 52 cards
    int index = 0;
    for (int suit = 0; suit < 4; suit++) { // create 13 cards for each of the 4 suits
        for (int rank = 1; rank <= 13; rank++) {
            cards[index] = Card(suit, rank);
            index++;
        }
    }
}

void Deck::shuffle() { // shuffles the remaining cards w/ the fisher yates algorithm
    static bool seeded = false; // keeps track of whether srand() alr called
    if (!seeded) { // only initialize the random number generator once
        srand(static_cast<unsigned int>(time(0))); // get time, initialize, then convert time to unsigned int
        seeded = true;
    }

    for (int i = deckCount - 1; i > 0; i--) { // swap each card with a randomly selected card
        int j = rand() % (i + 1);
        Card temp = cards[i];
        cards[i] = cards[j];
        cards[j] = temp;
    }
}

bool Deck::isEmpty() const { // checks if there are no cards left to draw
    return deckCount == 0;
}

int Deck::cardsRemaining() const { // returns the num of cards remaining in the deck
    return deckCount;
}

bool Deck::drawCard(Player& player) { // draws a card from the deck and adds it to the player's hand
    if (isEmpty() || player.isHandFull()) { // cant draw if the deck is empty or the player's hand is full
        return false;
    }

    if (player.addCard(cards[deckCount - 1])) { // use the player class's public fxn to add the card
        deckCount--; // remove the drawn card from the deck
        return true;
    }

    return false;
}

void Deck::discardCard(const Card& card) { // adds a played card to the discard pile
    // make sure the discard pile doesnt go over 52 cards
    if (discardedCount < 52) {
        discardPile[discardedCount] = card;
        discardedCount++;
    }
}
