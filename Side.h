//
//  Side.h
//  Kalah
//
//  Created by Yuan Shen on 5/17/19.
//  Copyright © 2019 Yuan Shen. All rights reserved.
//

#ifndef Side_h
#define Side_h

#include <iostream>

using namespace std;

enum Side { NORTH, SOUTH };

const int NSIDES = 2;
const int POT = 0;

inline
Side opponent(Side s)
{
    return Side(NSIDES - 1 - s);
}

#endif /* Side_hpp */
