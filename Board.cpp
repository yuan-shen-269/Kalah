//
//  Board.cpp
//  Kalah
//
//  Created by Yuan Shen on 5/17/19.
//  Copyright © 2019 Yuan Shen. All rights reserved.
//

#include "Board.h"

Board::Board(int nHoles, int nInitialBeansPerHole): numHoles(nHoles), numBeans(nInitialBeansPerHole){
    // set number of holes to 0 when it is not positive
    if(nHoles <= 0){
        numHoles = 1;
    }
    // set number of beans to 0 when it is negative
    if(nInitialBeansPerHole < 0){
        numBeans = 0;
    }
    
    // create the container of the board
    b = vector<int>(numHoles * 2 + 2, numBeans);
    b[numHoles] = 0;
    b[numHoles * 2 + 1] = 0;
}

int Board::holes() const{
    return numHoles;
}

int Board::beans(Side s, int hole) const{
    // when the hole number is invalid
    if(hole > numHoles || hole < 0){
        return -1;
    }
    // get the beans depend on side and whether it is a pot or a hole
    if(s == NORTH){
        if(hole == 0){
            return b[b.size()-1];
        }
        return b[numHoles*2+1 - hole];
    }
    else{
        if(hole == 0){
            return b[numHoles];
        }
        return b[hole-1];
    }
}

int Board::beansInPlay(Side s) const{
    int count = 0;
    // count the total number of beans in one side
    if(s == NORTH){
        for(int i = numHoles+1; i < b.size()-1; i++){
            count += b[i];
        }
    }
    else{
        for(int i = 0; i < numHoles; i++){
            count += b[i];
        }
    }
    return count;
}

int Board::totalBeans() const{
    int total = 0;
    for(int i = 0; i < numHoles*2+2; i++){
        total += b[i];
    }
    return total;
}

bool Board::sow(Side s, int hole, Side& endSide, int& endHole){
    // if the hole number is invalid
    if(hole > numHoles || hole <= 0){
        return false;
    }
    // get the number of beans in the indicated hole
    int countsow = 0;
    if(s == NORTH){
        countsow = b[numHoles*2+1 - hole];
        b[numHoles*2+1 - hole] = 0;
    }
    else{
        countsow = b[hole-1];
        b[hole-1] = 0;
    }
    // if there is nothing in the hole
    if(countsow == 0){
        return false;
    }
    int endindex = -1;
    // sow the beans until reaches the end of the vector the first time
    if(s == SOUTH){
        // starting to sow, skip the opponent's pot (the last element in the vector)
        for(int i = hole; i < b.size()-1; i++){
            b[i]++;
            countsow--;
            endindex = i;
            // stop sowing when the number of beans at hand reaches 0
            if(countsow == 0){
                break;
            }
        }
    }
    else{
        // starting to sow
        for(int i = numHoles*2+2 - hole; i < b.size(); i++){
            b[i]++;
            // skip the opponents pot
            if(i == numHoles){
                continue;
            }
            countsow--;
            endindex = i;
            // stop sowing when the number of beans at hand reaches 0
            if(countsow == 0){
                break;
            }
        }
    }
    // continue to sow the bean with full round
    while(countsow > 0){
        for(int i = 0; i < b.size(); i++){
            // skip the opponent's pot
            if((s == SOUTH && i == b.size()-1) || (s == NORTH && i == numHoles)){
                continue;
            }
            b[i]++;
            countsow--;
            endindex = i;
            // stop sowing when the number of beans at hand reaches 0
            if(countsow == 0){
                break;
            }
        }
    }
    // determine the end side and endhole
    if(endindex >= 0 && endindex < numHoles){
        endSide = SOUTH;
        endHole = endindex+1;
    }
    else if(endindex == numHoles){
        endSide = SOUTH;
        endHole = 0;
    }
    else if(endindex > numHoles && endindex < b.size()-1){
        endSide = NORTH;
        endHole = numHoles*2+1 - endindex;
    }
    else if(endindex == b.size()-1){
        endSide = NORTH;
        endHole = 0;
    }
    
    return true;
}

bool Board::moveToPot(Side s, int hole, Side potOwner){
    // if the hole number is in valid
    if(hole > numHoles || hole <= 0){
        return false;
    }
    
    int nh = 0;
    // get the number of beans in the indicated hole, and empty the hole
    if(s == NORTH){
        nh = b[numHoles*2+1 - hole];
        b[numHoles*2+1 - hole] = 0;
    }
    else{
        nh = b[hole-1];
        b[hole-1] = 0;
    }
    // add the beans into the player's pot
    if(potOwner == NORTH){
        b[b.size()-1] += nh;
    }
    else{
        b[numHoles] += nh;
    }
    return true;
}

bool Board::setBeans(Side s, int hole, int beans){
    // if the hole number and number of beans is invaid
    if(hole > numHoles || hole < 0 || beans < 0){
        return false;
    }
    // set the beans depend on side and whether it is a pot or a hole
    if(s == NORTH){
        if(hole == 0){
            b[numHoles*2+1] = beans;
        }
        b[numHoles*2+1 - hole] = beans;
    }
    else{
        if(hole == 0){
            b[numHoles] = beans;
        }
        b[hole-1] = beans;
    }
    return true;
}

