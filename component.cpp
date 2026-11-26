// component.cpp
// ENSC 251 Final Project - virus.exe
// Author: Jackson Brhelle
// Represents a single computer component 

#include "component.hpp"

using namespace std;

// Creates a component at full HP with no shield
Component::Component(string name, int maxHP) : name(name), currentHP(maxHP), maxHP(maxHP), shielded(false) {}
    
    
Component::~Component() {}

// Accessors

string Component::getName() const {
    return name;
}

int Component::getCurrentHP() const {
    return currentHP;
}

int Component::getMaxHP() const { 
    return maxHP;
}

bool Component::isDestroyed() const {
    return (currentHP == 0);
}

bool Component::isShielded() const {
    return shielded;
}

// Card effects

int Component::takeDamage(int amount) {
    
    // Shield blocks the attack, destroyed components cant be hit, and an amount below or equal to 0 does nothing
    if (shielded || isDestroyed() || amount <= 0) {
        return 0;
    }
    
    int damageDealt = amount;
    if (damageDealt > currentHP) {
        damageDealt = currentHP;    // stop at 0
    }
    
    currentHP = currentHP - damageDealt;
    return damageDealt;
}

int Component::heal(int amount) {
    
    // Destroyed components cant be healed
    if (isDestroyed() || amount <= 0) {
        return 0;
    }
    
    int hpRestored = amount;
    if (currentHP + hpRestored > maxHP) {
        hpRestored =  maxHP - currentHP;    // stop at max HP
    }
    
    currentHP = currentHP + hpRestored;
    return hpRestored;
}

void Component::activateShield() {
    
    // No effect on a destroyed component. If its already shielded, setting it to true does nothing
    if (!isDestroyed()) { 
        shielded = true;
    }
} 

void Component::removeShield() {
    shielded = false;
}

void Component::reset() { 
    currentHP = maxHP;
    shielded = false;
} 
