//
//  Grid.cpp
//  Tetriminos
//
//  Created by Tingting Gu on 2015/07/01.
//
//

#include "Grid.h"
#include "Tetromino.h"



bool Grid::init()
{
    if (! Sprite::init())
    {
        return false;
        
        // Tetromino* test = Tetromino::createWithType(TetrominoType::I);
    }
    
    return true;
}
