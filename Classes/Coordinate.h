//
//  Coordinate.h
//  Tetriminos
//
//  Created by Tingting Gu on 2015/07/01.
//
//

#ifndef __Tetriminos__Coordinate__
#define __Tetriminos__Coordinate__

#include "cocos2d.h"


class Coordinate
{
public:
    int x;
    int y;
    
    Coordinate();
    Coordinate(int x, int y);
    
    static Coordinate add(Coordinate a, Coordinate b);
};

#endif /* defined(__Tetriminos__Coordinate__) */
