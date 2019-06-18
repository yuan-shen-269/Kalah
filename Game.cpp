//
//  Game.cpp
//  Kalah
//
//  Created by Yuan Shen on 5/17/19.
//  Copyright © 2019 Yuan Shen. All rights reserved.
//

#include "Game.h"
Game::Game(const Board& b, Player* south, Player* north): board(b), s(south), n(north), curSide(SOUTH){}

void Game::display() const{
    // print the board
    int x = board.beans(NORTH, 0);
    int count = 0;
    while(true){
        count ++;
        x /= 10;
        if(x == 0){
            break;
        }
    }
    cout << endl;
    // north
    for(int i = 0; i < 15+count; i++){
        cout << " ";
    }
    for(int i = 0; i < (board.holes()*3 - 7)/2; i++){
        cout << " ";
    }
    cout << "North" << endl;
    // hole numbers for north
    for(int i = 0; i < 15+count; i++){
        cout << " ";
    }
    for(int i = 1; i <= board.holes(); i++){
        cout << i << "  ";
    }
    cout << endl;
    // dashes
    for(int i = 0; i < 15+count; i++){
        cout << " ";
    }
    for(int i = 0; i < board.holes()*3 - 2; i++){
        cout << "-";
    }
    cout << endl;
    // holes for north
    for(int i = 0; i < 15+count; i++){
        cout << " ";
    }
    for(int i = 1; i <= board.holes(); i++){
        cout << board.beans(NORTH, i) << "  ";
    }
    cout << endl;
    // pots
    cout << "North's pot  " << board.beans(NORTH, 0) << "  ";
    for(int i = 0; i < board.holes()*3 - 2; i++){
        cout << " ";
    }
    cout << "  " << board.beans(SOUTH, 0) << "  " << "South's pot" << endl;
    // holes for south
    for(int i = 0; i < 15+count; i++){
        cout << " ";
    }
    for(int i = 1; i <= board.holes(); i++){
        cout << board.beans(SOUTH, i) << "  ";
    }
    cout << endl;
    // dashes
    for(int i = 0; i < 15+count; i++){
        cout << " ";
    }
    for(int i = 0; i < board.holes()*3 - 2; i++){
        cout << "-";
    }
    cout << endl;
    // hole numbers for south
    for(int i = 0; i < 15+count; i++){
        cout << " ";
    }
    for(int i = 1; i <= board.holes(); i++){
        cout << i << "  ";
    }
    cout << endl;
    // south
    for(int i = 0; i < 15+count; i++){
        cout << " ";
    }
    for(int i = 0; i < (board.holes()*3 - 7)/2; i++){
        cout << " ";
    }
    cout << "South" << endl;
}

void Game::status(bool& over, bool& hasWinner, Side& winner) const{
    if (board.beansInPlay(NORTH) == 0 || board.beansInPlay(SOUTH) == 0){
        over = true;
        // if it is a tie
        if(board.beans(NORTH, 0) == board.beans(SOUTH, 0)){
            hasWinner = false;
        }
        // if north wins
        else if(board.beans(NORTH, 0) > board.beans(SOUTH, 0)){
            winner = NORTH;
            hasWinner = true;
        }
        // if south wins
        else{
            winner = SOUTH;
            hasWinner = true;
        }
    }
    else{
        over = false;
    }
}

bool Game::move(){
    bool thisOVer, thisHasWinner;
    Side thisWinner;
    // the player sow the seeds
    Side endSide;
    int endHole;
    if(curSide == SOUTH){
        board.sow(curSide, s->chooseMove(board, SOUTH), endSide, endHole);
    }
    else{
        board.sow(curSide, n->chooseMove(board, NORTH), endSide, endHole);
    }
    // if the last bean ends up in his own pot, move again
    while(endHole == 0 && endSide == curSide && board.beansInPlay(curSide) != 0 && board.beansInPlay(opponent(curSide)) != 0){
        cout << endl;
        display();
        cout << "Now it's ";
        if(curSide == SOUTH){
            cout << "South";
        }
        else{
            cout << "North";
        }
        cout << "'s turn." << endl;
        if(curSide == SOUTH){
            board.sow(curSide, s->chooseMove(board, SOUTH), endSide, endHole);
        }
        else{
            board.sow(curSide, n->chooseMove(board, NORTH), endSide, endHole);
        }
    }
    // if the player captures the bean
    if(endSide == curSide && beans(endSide, endHole) == 1 && endHole != 0) {
        if(beans(opponent(endSide), endHole) != 0){
            board.moveToPot(endSide, endHole, curSide);
            board.moveToPot(opponent(endSide), endHole, curSide);
        }
    }
    // if the game is over, the player do nothing, move all the beans in the other side to the other player
    status(thisOVer, thisHasWinner, thisWinner);
    if(thisOVer){
        for(int i = 1; i <= board.holes(); i++){
            board.moveToPot(NORTH, i, NORTH);
            board.moveToPot(SOUTH, i, SOUTH);
        }
        return false;
    }
    // change the turn to the opponent
    curSide = opponent(curSide);
    return true;
}

void Game::play(){
    // print the player names
    cout << "Player South's name: " << s->name() << endl;
    cout << "Player North's name: " << n->name() << endl << endl;
    bool thisOVer, thisHasWinner;
    Side thisWinner;
    // get the status of the game
    status(thisOVer, thisHasWinner, thisWinner);
    // if the game is over, the player do nothing, move all the beans in the other side to the other player
    if(thisOVer){
        for(int i = 1; i <= board.holes(); i++){
            board.moveToPot(NORTH, i, NORTH);
            board.moveToPot(SOUTH, i, SOUTH);
        }
    }
    int prompt = 0;
    while(true){
        prompt++;
        
        status(thisOVer, thisHasWinner, thisWinner);
        
        if(thisOVer){
            break;
        }
        
        display();
        
        cout << "Now it's ";
        if(curSide == SOUTH){
            cout << "South";
        }
        else{
            cout << "North";
        }
        cout << "'s turn." << endl;
        
        move();
        
        cout << endl;
        
        if(!n->isInteractive() && !s->isInteractive() && prompt % 10 == 0){
            cout << "Press Enter to continue: ";
            cin.ignore();
            cout << endl;
        }
    }
    
    display();
    
    cout << "Game is over." << endl;
    
    if(thisHasWinner){
        std::string winnerName;
        if(thisWinner == SOUTH){
            winnerName = s->name();
        }
        else{
            winnerName = n->name();
        }
        cout << "The winner is " << winnerName << "." << endl;
    }
    else{
        cout << "It's a tie." << endl;
    }
}

int Game::beans(Side s, int hole) const{
    return board.beans(s, hole);
}
