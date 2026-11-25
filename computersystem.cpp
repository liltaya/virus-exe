// computersystem.cpp
// ENSC 251 Final Project - virus.exe
// Author: Jackson Brhelle
//
// Represents a player's whole computer system: the four components
// CPU (20 HP), Memory (10 HP), Network (5 HP), and Storage (15 HP).
// Gives access to the component matching a card's suit, calculates total
// remaining HP, and reports whether the whole system is destroyed.
//
// Suits are passed as ints, using the same numbering as the Card class:
//   0 = HEARTS   -> CPU
//   1 = DIAMONDS -> Memory
//   2 = CLUBS    -> Network
//   3 = SPADES   -> Storage

#include "computersystem.hpp"

// Creates the four components at full HP, in suit order:
// 0 = CPU (20), 1 = MEMORY (10), 2 = NETWORK (5), 3 = STORAGE (15)
ComputerSystem::ComputerSystem() {
    component.push_back(Component("CPU", 20));
    component.push_back(Component("MEMORY", 10));
    component.push_back(Component("NETWORK", 5));
    component.push_back(Component("STORAGE", 15));
}

// Destructor
ComputerSystem::~ComputerSystem() {}

// Accessors 

// Precondition 0 <= suit <= 3
Component& ComputerSystem::getComponent(int suit) {
    return component[suit];
}

// Precondition 0 <= suit <= 3
const Component& ComputerSystem::getComponent(int suit) const {
    return component[suit];
}

// System status

int ComputerSystem::getTotalHP() const {
    int total = 0;
    for (unsigned int i = 0; i < component.size(); i++) {
        total = total + component[i].getCurrentHP();
    }
    return total;
}

bool ComputerSystem::isSystemDestroyed() const {
    for (unsigned int i = 0; i < component.size(); i++) { 
        if (!component[i].isDestroyed()) {
            return false;   // at least one component is still alive
        }
    }
    return true;
}

// Turn and game management

void ComputerSystem::removeAllShields() { 
    for (unsigned int i = 0; i < component.size(); i++) { 
        component[i].removeShield();
    } 
} 

void ComputerSystem::reset() { 
    for (unsigned int i = 0; i < component.size(); i++) {
        component[i].reset();
    }
}