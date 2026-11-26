// Alina Beg
// 301593470
// ENSC 251: Software Design and Analysis for Engineers
// virus.exe Card Class

#ifndef CARD_H
#define CARD_H

#include <string>
using namespace std;

class Card {
private:
    int suit; 
    int rank; 

public:
    Card();
    Card(int s, int r);

    int getSuit() const; 
    int getRank() const;
    int getEffectValue() const; // get number for effect value ex. jack (11) returns 2
    string getDescription() const; // get description of card in readable form ex. jack of hearts
};

#endif
