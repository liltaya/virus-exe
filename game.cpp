//Taya Mamenko
//301649208
//ENSC 251: Software Design and Analysis for Engineers
//virus.exe Game Class

#include "game.hpp" //include Game class
#include "gameui.hpp" //include GameUI class
#include <stdexcept> //for reporting errors

using namespace std;

Game::Game(
    const string& player1Name,
    const string& player2Name,
    GameUI& gameUI)
    : player1(player1Name), //create first player
      player2(player2Name), //create second player
      deck(), //create game deck
      ui(gameUI), //store interface reference
      currentPlayer(1), //first player starts
      gameOver(false), //game is not finished
      winner(""), //no winner yet
      turnsCompleted(0) //no turns completed
{
}

Player& Game::getCurrentPlayer() { //get current player

    if (currentPlayer == 1) { //check first player

        return player1; //return first player
    }
    else { //second player's turn

        return player2; //return second player
    }
}

Player& Game::getPlayer(int playerNumber) { //get selected player

    if (playerNumber == 1) { //check first player

        return player1; //return first player
    }
    else if (playerNumber == 2) { //check second player

        return player2; //return second player
    }

    throw out_of_range("Invalid player number"); //reject number
}

Deck& Game::getDeck() { //get game deck

    return deck; //return deck reference
}

bool Game::addCard() { //draw one card

    if (gameOver) { //check if game ended

        return false; //cannot draw after game ends
    }

    if (getCurrentPlayer().isHandFull()) { //check hand limit

        return false; //maximum 5 cards in hand
    }

    if (deck.isEmpty()) { //check remaining cards

        return false; //deck has no cards
    }

    return deck.drawCard(getCurrentPlayer()); //draw card into hand
}

bool Game::isValidMove(int cardIndex) { //check selected card

    if (gameOver) { //check game status

        return false; //game already ended
    }

    Player& player = getCurrentPlayer(); //get current player

    if (cardIndex < 0) { //check negative index

        return false; //invalid card index
    }

    if (static_cast<size_t>(cardIndex) >= player.getHandSize()) { //check hand size

        return false; //card is not in hand
    }

    const Card& card =
        player.getHand().at(cardIndex); //get selected card

    int suit = card.getSuit(); //get card suit

    int rank = card.getRank(); //get card rank

    if (suit < 0 || suit > 3 || rank < 1 || rank > 13) { //check card values

        return false; //invalid suit or rank
    }

    Player& opponent = getPlayer(
        currentPlayer == 1 ? 2 : 1 //get opposing player number
    );

    if (rank >= 2 && rank <= 10) { //check attack card

        return !opponent.getSystem()
                        .getComponent(suit) //get opponent component
                        .isDestroyed(); //check if target is alive
    }

    return !player.getSystem()
                  .getComponent(suit) //get own component
                  .isDestroyed(); //check if target is alive
}

void Game::applyCardEffect(const Card& card) { //apply card effect

    int suit = card.getSuit(); //get card suit

    int rank = card.getRank(); //get card rank

    int value = card.getEffectValue(); //get effect value

    Player& player = getCurrentPlayer(); //get current player

    Player& opponent = getPlayer(
        currentPlayer == 1 ? 2 : 1 //get opposing player number
    );

    if (rank >= 2 && rank <= 10) { //check attack card

        Component& target =
            opponent.getSystem().getComponent(suit); //get target component

        if (target.isShielded()) { //check active shield

            ui.displayMessage(
                target.getName() + " blocked the attack." //show blocked attack
            );
        }
        else { //component has no shield

            int previousHP =
                target.getCurrentHP(); //store HP before attack

            target.takeDamage(value); //apply damage

            int damage =
                previousHP - target.getCurrentHP(); //calculate actual damage

            ui.displayMessage(
                opponent.getName() + "'s " + //get opponent name
                target.getName() + " took " + //get component name
                to_string(damage) + " damage." //show damage dealt
            );

            ui.displayMessage(
                target.getName() + " HP: " + //show target component
                to_string(previousHP) + " -> " + //show previous HP
                to_string(target.getCurrentHP()) //show remaining HP
            );

            if (target.isDestroyed()) { //check component status

                ui.displayMessage(
                    target.getName() + " was destroyed." //show destroyed component
                );
            }
        }
    }

    else if (rank >= 11 && rank <= 13) { //check healing card

        Component& target =
            player.getSystem().getComponent(suit); //get own component

        int previousHP =
            target.getCurrentHP(); //store HP before healing

        target.heal(value); //restore component HP

        int restored =
            target.getCurrentHP() - previousHP; //calculate restored HP

        ui.displayMessage(
            player.getName() + "'s " + //get player name
            target.getName() + " restored " + //get component name
            to_string(restored) + " HP." //show restored HP
        );

        ui.displayMessage(
            target.getName() + " HP: " + //show healed component
            to_string(previousHP) + " -> " + //show previous HP
            to_string(target.getCurrentHP()) //show current HP
        );
    }

    else if (rank == 1) { //check shield card

        Component& target =
            player.getSystem().getComponent(suit); //get own component

        target.activateShield(); //activate component shield

        ui.displayMessage(
            player.getName() + "'s " + //get player name
            target.getName() + " is now shielded." //show active shield
        );
    }
}

bool Game::removeCard(int cardIndex) { //play selected card

    if (!isValidMove(cardIndex)) { //check selected move

        return false; //reject invalid move
    }

    Player& player = getCurrentPlayer(); //get current player

    Card card =
        player.getHand().at(cardIndex); //copy selected card

    Card removedCard = card; //store removed card

    if (!player.removeCard(cardIndex, removedCard)) { //remove card from hand

        return false; //card removal failed
    }

    ui.displayMessage(
        player.getName() + " played " + //get player name
        card.getDescription() + "." //show played card
    );

    applyCardEffect(card); //apply selected card effect

    deck.discardCard(removedCard); //move card to discard pile

    return true; //card played successfully
}

void Game::removeExpiredShields() { //remove expired shields

    getCurrentPlayer()
        .getSystem()
        .removeAllShields(); //remove shields at start of owner's turn
}

void Game::checkWinner() { //check if game ended

    bool player1Destroyed =
        player1.isSystemDestroyed(); //check first system

    bool player2Destroyed =
        player2.isSystemDestroyed(); //check second system

    if (player1Destroyed && player2Destroyed) { //both systems destroyed

        winner = "Draw"; //game ends in draw

        gameOver = true; //finish game

        return; //stop checking
    }

    if (player1Destroyed) { //first system destroyed

        winner = player2.getName(); //second player wins

        gameOver = true; //finish game

        return; //stop checking
    }

    if (player2Destroyed) { //second system destroyed

        winner = player1.getName(); //first player wins

        gameOver = true; //finish game

        return; //stop checking
    }

    if (deck.isEmpty()) { //check empty deck

        int hp1 = player1.getTotalHP(); //get first player HP

        int hp2 = player2.getTotalHP(); //get second player HP

        if (hp1 > hp2) { //first player has more HP

            winner = player1.getName(); //first player wins
        }
        else if (hp2 > hp1) { //second player has more HP

            winner = player2.getName(); //second player wins
        }
        else { //equal remaining HP

            winner = "Draw"; //game ends in draw
        }

        gameOver = true; //finish game
    }
}

void Game::nextTurn() { //switch to next player

    if (gameOver) { //check if game finished

        return; //stop turn switching
    }

    turnsCompleted++; //record completed turn

    if (player1.isSystemDestroyed() ||
        player2.isSystemDestroyed()) { //check destroyed systems

        checkWinner(); //determine game winner

        return; //stop turn switching
    }

    if (currentPlayer == 1) { //first player's turn ended

        currentPlayer = 2; //switch to second player
    }
    else { //second player's turn ended

        checkWinner(); //check winner after full round

        if (gameOver) { //check if game ended

            return; //stop switching players
        }

        currentPlayer = 1; //switch to first player
    }

    removeExpiredShields(); //update shield status

    ui.displayTurnDivider(
        getCurrentPlayer().getName()
    ); //hide previous player's cards and wait for next player
}

void Game::playTurn() { //handle current player's turn

    if (gameOver) { //check game status

        return; //cannot play after game ends
    }

    Player& player = getCurrentPlayer(); //get current player

    ui.displayTurn(player.getName()); //show current player

    ui.displayComponents(
        player1.getName(), //first player's name
        player1.getSystem() //show first player's components
    );

    ui.displayComponents(
        player2.getName(), //second player's name
        player2.getSystem() //show second player's components
    );

    ui.displayHand(player.getHand()); //show player hand

    bool canDraw =
        !player.isHandFull() && !deck.isEmpty(); //check draw action

    bool canPlay =
        player.hasCards(); //check play action

    ui.displayTurnMenu(canDraw, canPlay); //show available actions

    int choice =
        ui.getTurnChoice(canDraw, canPlay); //read selected action

    if (choice == 0) { //no action available

        ui.displayMessage(
            "No actions available." //show message
        );

        nextTurn(); //skip current turn

        return; //finish action
    }

    if (choice == 1) { //player chooses to draw

        if (addCard()) { //attempt to draw card

            ui.displayMessage(
                "Card drawn successfully." //confirm successful draw
            );

            nextTurn(); //finish turn
        }
        else { //draw failed

            ui.displayMessage(
                "Cannot draw card." //show error
            );
        }

        return; //finish action
    }

    if (choice == 2) { //player chooses to play

        int cardIndex = ui.getCardChoice(
            static_cast<int>(player.getHandSize()) //select card from hand
        );

        if (removeCard(cardIndex)) { //attempt to play card

            ui.displayMessage(
                "Card played successfully." //confirm successful play
            );

            nextTurn(); //finish turn
        }
        else { //invalid card

            ui.displayMessage(
                "Invalid move. Try again." //show error
            );
        }

        return; //finish action
    }
}

GameResult Game::getResult() const { //get final game result

    if (!gameOver) { //check game status

        throw logic_error("Game is not finished"); //reject request
    }

    if (winner == "Draw") { //check draw

        return GameResult::Draw; //return draw result
    }

    if (winner == player1.getName()) { //check first player

        return GameResult::Player1Win; //return first player win
    }

    if (winner == player2.getName()) { //check second player

        return GameResult::Player2Win; //return second player win
    }

    throw logic_error("Unknown game result"); //reject unknown result
}

GameResult Game::startGame() { //start a new game

    currentPlayer = 1; //first player starts

    gameOver = false; //reset game status

    winner = ""; //clear previous winner

    turnsCompleted = 0; //reset turn counter

    player1 = Player(player1.getName()); //reset first player

    player2 = Player(player2.getName()); //reset second player

    deck = Deck(); //create fresh deck

    deck.shuffle(); //shuffle cards

    for (int i = 0; i < 5; i++) { //deal starting hands

        if (!deck.isEmpty()) { //check remaining cards

            deck.drawCard(player1); //give card to first player
        }

        if (!deck.isEmpty()) { //check remaining cards

            deck.drawCard(player2); //give card to second player
        }
    }

    while (!gameOver) { //continue until game ends

        playTurn(); //handle current player's action
    }

    ui.displayMessage(
        "\n--- FINAL SYSTEM STATUS ---" //show final system summary
    );

    ui.displayComponents(
        player1.getName(), //first player's name
        player1.getSystem() //show final first player HP
    );

    ui.displayComponents(
        player2.getName(), //second player's name
        player2.getSystem() //show final second player HP
    );

    return getResult(); //return final result to main
}

bool Game::isGameOver() const { //check game status

    return gameOver; //return true if game finished
}

string Game::getWinner() const { //get winning player

    return winner; //return winner name or draw
}
