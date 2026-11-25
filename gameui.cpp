//Taya Mamenko
//301649208
//ENSC 251: Software Design and Analysis for Engineers
//virus.exe GameUI Class

#include "gameui.hpp" //include GameUI class
#include "game.hpp" //include GameResult definition
#include <iostream> //for input and output
#include <sstream> //for converting text into number
#include <stdexcept> //for input errors
#include <string> //for storing user input

using namespace std;

GameUI::GameUI() { //default constructor
    //interface has no stored game object
}

void GameUI::displayTitleScreen() { //show main menu

    cout << "\n-- VIRUS.EXE --" << endl; //show game title
    cout << "1. Player vs. Player" << endl; //show game option
    cout << "2. Rules" << endl; //show rules option
    cout << "3. Quit" << endl; //show quit option
}

int GameUI::getValidInput(int min, int max) { //check user input

    string input; //store entered text
    int choice; //store choice number

    while (true) { //repeat until input is valid

        if (!getline(cin, input)) { //read user input
            throw runtime_error("Input stream closed"); //stop on EOF
        }

        istringstream inputStream(input); //convert text to number
        char extra; //check for extra characters

        if ((inputStream >> choice) &&
            !(inputStream >> extra)) { //accept only whole numbers

            if (choice >= min && choice <= max) { //check range
                return choice; //return valid choice
            }
        }

        displayInvalidInput(); //show invalid input message
    }
}

string GameUI::getSuitName(int suit) const { //get suit name

    if (suit == 0) { //check hearts

        return "Hearts"; //hearts suit
    }
    else if (suit == 1) { //check diamonds

        return "Diamonds"; //diamonds suit
    }
    else if (suit == 2) { //check clubs

        return "Clubs"; //clubs suit
    }
    else if (suit == 3) { //check spades

        return "Spades"; //spades suit
    }

    return "Unknown"; //invalid suit
}

string GameUI::getComponentName(int suit) const { //get component for suit

    if (suit == 0) { //hearts target

        return "CPU"; //hearts hits CPU
    }
    else if (suit == 1) { //diamonds target

        return "Memory"; //diamonds hits memory
    }
    else if (suit == 2) { //clubs target

        return "Network"; //clubs hits network
    }
    else if (suit == 3) { //spades target

        return "Storage"; //spades hits storage
    }

    return "Unknown"; //invalid suit
}

string GameUI::getCardAction(
    const Card& card,
    const ComputerSystem& ownSystem,
    const ComputerSystem& opponentSystem) const { //describe card effect

    int suit = card.getSuit(); //get card suit

    int rank = card.getRank(); //get card rank

    if (suit < 0 || suit > 3) { //check suit range

        return "no target"; //invalid card
    }

    string component = getComponentName(suit); //get target component

    if (rank >= 2 && rank <= 10) { //check attack card

        if (opponentSystem.getComponent(suit).isDestroyed()) { //check target status

            return "opponent " + component +
                   " destroyed, discard only"; //show dead target
        }

        return "attack opponent " + component +
               " for " + to_string(card.getEffectValue()); //show attack
    }

    if (ownSystem.getComponent(suit).isDestroyed()) { //check own target status

        return "your " + component +
               " destroyed, discard only"; //show dead target
    }

    if (rank >= 11 && rank <= 13) { //check healing card

        return "heal your " + component +
               " +" + to_string(card.getEffectValue()); //show heal
    }

    return "shield your " + component; //ace shields component
}

int GameUI::getMenuChoice() { //get main menu selection

    cout << "Enter your choice: "; //ask for menu option

    return getValidInput(1, 3); //accept options 1 to 3
}

string GameUI::getPlayerName(int playerNumber) { //get player name

    string name; //store player name

    cout << "Enter Player " << playerNumber
         << " name: "; //ask for player name

    if (!getline(cin, name)) { //read name
        throw runtime_error("Input stream closed"); //stop on EOF
    }

    if (name.empty()) { //check for empty name
        return "Player " + to_string(playerNumber); //use default
    }

    return name; //return entered name
}

void GameUI::displayRules() { //show game rules

    cout << "\n--- GAME RULES ---" << endl; //show rules title

    cout << "Each player has 4 computer components." << endl; //show system structure
    cout << "Each card suit targets one component." << endl; //show suit link

    cout << "\nHearts   -> CPU: 20 HP" << endl; //show CPU suit and health
    cout << "Diamonds -> Memory: 10 HP" << endl; //show memory suit and health
    cout << "Clubs    -> Network: 5 HP" << endl; //show network suit and health
    cout << "Spades   -> Storage: 15 HP" << endl; //show storage suit and health

    cout << "\nEach player starts with 5 cards." << endl; //show starting hand
    cout << "Draw or play one card per turn." << endl; //show turn rules

    cout << "\nCards 2-10 deal damage to the opponent's component." << endl; //show attack cards
    cout << "Jack heals 2 HP on your own component." << endl; //show Jack effect
    cout << "Queen heals 4 HP on your own component." << endl; //show Queen effect
    cout << "King heals 6 HP on your own component." << endl; //show King effect
    cout << "Ace shields your own component for one turn." << endl; //show Ace effect
    cout << "Destroyed components cannot be healed or shielded." << endl; //show destroyed rule
    cout << "Cards for a destroyed component are discarded automatically." << endl; //show discard rule
    cout << "If you draw one, you draw again. This does not use your turn." << endl; //show redraw rule

    cout << "\nDestroy all opponent components to win." << endl; //show win condition
    cout << "When the deck is empty, keep playing your hand." << endl; //show empty deck rule
    cout << "Once the deck and both hands are empty, highest HP wins." << endl; //show deck condition
}

void GameUI::displayGameResult(GameResult result) { //show result

    cout << "\n--- GAME OVER ---" << endl; //show game ending

    if (result == GameResult::Player1Win) { //first player wins

        cout << "Winner: Player 1" << endl; //show first player
    }
    else if (result == GameResult::Player2Win) { //second player wins

        cout << "Winner: Player 2" << endl; //show second player
    }
    else { //both players have equal result

        cout << "The game is a draw." << endl; //show draw
    }
}

void GameUI::displayGameEndMenu() { //show end menu

    cout << "\n--- GAME FINISHED ---" << endl; //show end menu title

    cout << "1. Play Again" << endl; //show replay option
    cout << "2. Quit" << endl; //show quit option
}

int GameUI::getGameEndChoice() { //read end menu choice

    cout << "Enter your choice: "; //ask for selection

    return getValidInput(1, 2); //accept replay or quit
}

void GameUI::displayInvalidInput() { //show input error

    cout << "Invalid input. Try again." << endl; //show error message
}

void GameUI::displayGoodbye() { //show goodbye message

    cout << "\n--- END OF PROGRAM ---" << endl; //show program ending
}

void GameUI::displayTurn(
    const string& playerName) { //show current turn

    cout << "\n--- CURRENT TURN ---" << endl; //show turn title

    cout << "Player: "
         << playerName << endl; //show player name
}

void GameUI::displayTurnDivider(
    const string& playerName) { //separate player turns

    cout << "\nPress Enter to finish your turn..."; //allow player to read action result

    string input; //store player input

    if (!getline(cin, input)) { //wait for current player
        throw runtime_error("Input stream closed"); //stop on EOF
    }

    cout << string(60, '\n'); //hide previous player's cards

    cout << "\033[2J\033[H"; //clear visible terminal screen

    cout << string(50, '=') << endl; //show divider line

    cout << "\n           TURN COMPLETED\n" << endl; //show completed turn

    cout << "Pass the computer to "
         << playerName << endl; //show next player

    cout << "\nPress Enter when ready...\n"; //wait for next player

    cout << string(50, '=') << endl; //show bottom divider

    if (!getline(cin, input)) { //wait for next player's confirmation
        throw runtime_error("Input stream closed"); //stop on EOF
    }

    cout << "\033[2J\033[H"; //clear transition screen

    cout << string(50, '=') << endl; //show new turn divider
}

void GameUI::displayComponents(
    const string& playerName,
    const ComputerSystem& system) { //show component health summary

    cout << "\n--- "
         << playerName
         << " SYSTEM ---" << endl; //show player system

    int totalHP = 0; //store combined current HP
    int maxHP = 0; //store combined maximum HP

    for (int i = 0; i < 4; i++) { //display all four components

        const Component& component =
            system.getComponent(i); //get current component

        cout << component.getName() //show component name
             << " (" << getSuitName(i) << "): " //show matching suit
             << component.getCurrentHP() << "/" //show current HP
             << component.getMaxHP() << " HP"; //show maximum HP

        if (component.isDestroyed()) { //check destroyed component

            cout << " [DESTROYED]"; //show destroyed status
        }
        else if (component.isShielded()) { //check active shield

            cout << " [SHIELDED]"; //show shield status
        }

        cout << endl; //move to next component

        totalHP += component.getCurrentHP(); //add current component HP

        maxHP += component.getMaxHP(); //add maximum component HP
    }

    cout << "Total HP: "
         << totalHP << "/"
         << maxHP << " HP" << endl; //show combined system HP
}

void GameUI::displayHand(
    const vector<Card>& hand,
    const ComputerSystem& ownSystem,
    const ComputerSystem& opponentSystem) { //show player hand

    cout << "\n--- YOUR CARDS ---" << endl; //show hand title

    cout << "Cards in hand: "
         << hand.size() << endl; //show hand size

    if (hand.empty()) { //check for empty hand

        cout << "Your hand is empty." << endl; //show empty hand

        return; //stop displaying cards
    }

    for (size_t i = 0; i < hand.size(); i++) { //display each card

        cout << i + 1 << ". " //show card number
             << hand[i].getDescription() //show card rank and suit
             << " -> "
             << getCardAction(
                    hand[i],
                    ownSystem,
                    opponentSystem) //show card effect and target
             << endl;
    }
}

void GameUI::displayTurnMenu(
    bool canDraw,
    bool canPlay) { //show available turn actions

    cout << "\n--- CHOOSE ACTION ---" << endl; //show action menu

    if (canDraw) { //drawing is available

        cout << "1. Draw Card" << endl; //show draw option
    }

    if (canPlay) { //playing is available

        cout << "2. Play Card" << endl; //show play option
    }

    if (!canDraw && !canPlay) { //no action available

        cout << "No actions available." << endl; //show unavailable actions
    }
}

int GameUI::getTurnChoice(
    bool canDraw,
    bool canPlay) { //read current player's action

    if (!canDraw && !canPlay) { //check for unavailable actions

        return 0; //skip current turn
    }

    while (true) { //repeat until action is valid

        cout << "Choose action: "; //ask for action

        int choice = getValidInput(1, 2); //read action

        if (choice == 1 && canDraw) { //check draw action

            return 1; //return draw choice
        }

        if (choice == 2 && canPlay) { //check play action

            return 2; //return play choice
        }

        displayInvalidInput(); //reject unavailable action
    }
}

int GameUI::getCardChoice(int handSize) { //select card

    if (handSize <= 0) { //check for empty hand

        return -1; //no valid card index
    }

    cout << "Select card to play (1-"
         << handSize << "): "; //show available card numbers

    return getValidInput(1, handSize) - 1; //convert to index
}

void GameUI::displayMessage(
    const string& message) { //show game message

    cout << message << endl; //print message
}
