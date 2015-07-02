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
    if (! Sprite::initWithFile("grid.png"))
    {
        return false;
    }
    
    // test JSON Data
        // Tetromino* test = Tetromino::createWithType(TetrominoType::I);
    
    // test Block
        // Tetromino* testBlock = Tetromino::createWithType(TetrominoType::Z);
        // this->addChild(testBlock);
    
    return true;
}