//
//  Player.cpp
//  Kalah
//
//  Created by Yuan Shen on 5/17/19.
//  Copyright © 2019 Yuan Shen. All rights reserved.
//

#include "Player.h"
#include <cstdlib>
#include <ctime>
#include <limits>

Player::Player(std::string name): p_name(name){
}

std::string Player::name() const{
    return p_name;
}

bool Player::isInteractive() const{
    return false;
}

Player::~Player(){}

HumanPlayer::HumanPlayer(std::string name): Player(name){}

bool HumanPlayer::isInteractive() const{
    return true;
}

int HumanPlayer::chooseMove(const Board& b, Side s) const{
    int x;
    while(true){
        cout << "Please choose a hole (1-" << b.holes() << "): ";
        cin >> x;
        // if the hole number is valid, stop prompting for a new hole number
        if(x >= 1 && x<=b.holes() && b.beans(s, x) != 0){
            break;
        }
        cin.ignore();
    }
    return x;
}

HumanPlayer::~HumanPlayer(){}


BadPlayer::BadPlayer(std::string name): Player(name){}

int BadPlayer::chooseMove(const Board& b, Side s) const{
    // set the seed for rand()
    srand((time(0)));
    // generate a random hole number between 1 to numHole
    int hole = 1 + rand() % b.holes();
    // generate the hole number until it is valid
    while(b.beans(s, hole) == 0){
        hole = 1 + rand() % b.holes();
    }
    return hole;
}

BadPlayer::~BadPlayer(){}

SmartPlayer::SmartPlayer(std::string name): Player(name){}


int SmartPlayer::chooseMove(const Board& b, Side s) const{
    int bestHole, value, depth = 0;
    smartMove(b, s, bestHole, value, depth);
    return bestHole;
}

SmartPlayer::~SmartPlayer(){}

void SmartPlayer::smartMove(Board b, Side s, int& bestHole, int& value, int depth) const{
    value = b.beans(SOUTH, 0) - b.beans(NORTH, 0);
    if(depth == 8){
        return;
    }
    // if there is no move (base case)
    if(b.beansInPlay(NORTH) == 0 || b.beansInPlay(SOUTH) == 0){
        for(int i = 0; i < b.holes(); i++){
            b.moveToPot(SOUTH, i, SOUTH);
            b.moveToPot(NORTH, i, NORTH);
        }
        bestHole = -1;
        // if the South player wins
        if(b.beans(SOUTH, 0) > b.beans(NORTH, 0)){
            value = std::numeric_limits<int>::max();
        }
        // if the South player loses
        else if(b.beans(SOUTH, 0) < b.beans(NORTH, 0)){
            value = std::numeric_limits<int>::min();
        }
        // if it is a tie
        else{
            value = 0;
        }
        return;
    }
    // find the first valid index
    int firstValid = -1;
    for(int i = 1; i <= b.holes();i++){
        if(b.beans(s, i) != 0){
            firstValid = i;
            break;
        }
    }
    int interValue = 0, interHole = -1;
    for(int i = 1; i <= b.holes(); i++){
        // if there is no bean in the hole, skip the hole
        if(b.beans(s, i) == 0){
            continue;
        }
        // copy a new Board and sow once
        Side endSide;
        int endHole, newHole = 0, newValue = 0;
        Board newBoard = b;
        newBoard.sow(s, i, endSide, endHole);
        // if the player captures the bean
        if(endSide == s && newBoard.beans(endSide, endHole) == 1 && endHole != 0) {
            if(newBoard.beans(opponent(endSide), endHole) != 0){
                newBoard.moveToPot(endSide, endHole, s);
                newBoard.moveToPot(opponent(endSide), endHole, s);
            }
        }
        // if the last beans end up in his own pot, move again
        if(endHole == 0 && endSide == s){
            smartMove(newBoard, s, newHole, newValue, depth+1);
        }
        // find the bestValue and bestHole for the current status
        else{
            smartMove(newBoard, opponent(s), newHole, newValue, depth+1);
        }
        // initialize the interValue and interHole with the information of the first hole that can sow in the next step
        if(i == firstValid){
            interValue = newValue;
            interHole = i;
        }
        // if SOUTH is playing and the newValue is bigger than any other value so far, set the value to newValue, when SOUTH wins, stop exploring other steps in this turn
        if(s == SOUTH && newValue >= interValue){
            interValue = newValue;
            interHole = i;
            if(value == std::numeric_limits<int>::max()){
                break;
            }
        }
        // if NORTH is playing and the newValue is smaller than any other value so far, set the value to newValue, when SOUTH wins, stop exploring other steps in this turn
        else if(s == NORTH && newValue <= interValue){
            interValue = newValue;
            interHole = i;
            if(value == std::numeric_limits<int>::min()){
                break;
            }
        }
    }
    value = interValue;
    bestHole = interHole;
    return;
}




