//
//  Player.h
//  Kalah
//
//  Created by Yuan Shen on 5/17/19.
//  Copyright © 2019 Yuan Shen. All rights reserved.
//

#include "Side.h"
#include "Board.h"

#ifndef Player_h
#define Player_h

//Player is an abstract base class that defines a common interface that all kinds of players (human and various computer players) must implement. It must support these public member functions:

class Player{
public:
    Player(std::string name);
    //Create a Player with the indicated name.
    std::string name() const;
    //Return the name of the player.
    virtual bool isInteractive() const;
    //Return false if the player is a computer player. Return true if the player is human. Most kinds of players will be computer players.
    virtual int chooseMove(const Board& b, Side s) const = 0;
    //Every concrete class derived from this class must implement this function so that if the player were to be playing side s and had to make a move given board b, the function returns the move the player would choose. If no move is possible, return −1.
    virtual ~Player();
    //Since this class is designed as a base class, it should have a virtual destructor.
private:
    std::string p_name;
};



//Each concrete class derived from Player will implement the chooseMove function in its own way. Of the classes listed here, only HumanPlayer::isInteractive should return true. (When testing, we may supply other kinds of interactive players.) Each of the three classes listed here must have a constructor taking a string representing the name of the player.

class HumanPlayer: public Player{
public:
    HumanPlayer(std::string name);
    bool isInteractive() const;
    int chooseMove(const Board& b, Side s) const;
    ~HumanPlayer();
};
//A HumanPlayer chooses its move by prompting a person running the program for a move (reprompting if necessary until the person enters a valid hole number), and returning that choice. We will never test for a situation where the user doesn't enter an integer when prompted for a hole number. (The techniques for dealing with the issue completely correctly are a distraction to this project, and involve either a function like stoi or strtol, or the type istringstream.)

class BadPlayer: public Player{
public:
    BadPlayer(std::string name);
    int chooseMove(const Board& b, Side s) const;
    ~BadPlayer();
};
//A BadPlayer is a computer player that chooses an arbitrary valid move and returns that choice. "Arbitrary" can be what you like: leftmost, nearest to pot, fewest beans, random, etc.. The point of this class is to have an easy-to-implement class that at least plays legally.

class SmartPlayer: public Player{
public:
    SmartPlayer(std::string name);
    int chooseMove(const Board& b, Side s) const;
    ~SmartPlayer();
private:
    void smartMove(Board b, Side s, int& bestHole, int& value, int depth) const;
};
//Here's your chance to shine. A SmartPlayer chooses a valid move and returns it. For any game played on a board of up to six holes per side, with up to four initial beans per hole, SmartPlayer::chooseMove must return its choice in no more than five seconds. (We'll give you a way of determining the time soon; until then, you can meet this requirement by limiting the depth of your game tree search or the number of game tree positions you explore to a limit you determine experimentally.) SmartPlayer::chooseMove will be worth about 15-20% of the points for this project.

#endif /* Player_hpp */
