//
//  Grid.cpp
//  Tetriminos
//
//  Created by Tingting Gu on 2015/07/01.
//
//

#include "Grid.h"
#include "Tetromino.h"
#include "Constants.h"

using namespace cocos2d;


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
    
    // test Block's retation
        // Tetromino* rotateBlock = Tetromino::createWithType(TetrominoType::I);
        // this->addChild(rotateBlock);
        // rotateBlock->rotate(true);
    
    this->activeTetromino = nullptr; // = 0
    
    return true;
}

void Grid::onEnter()
{
    Sprite::onEnter();
    
    //this->activeTetromino = Tetromino::createWithType(TetrominoType::L);
    //this->addChild(activeTetromino);
}

void Grid::rotateActiveTetromino()
{
    if (this->activeTetromino)
    {
        this->activeTetromino->rotate(true);
    }
    
    //Check if collision, undo rotation

}

void Grid::spawnTetromino(Tetromino *tetromino)
{
    this->activeTetromino = tetromino;
    this->addChild(this->activeTetromino);
    
    // TODO: Place tetromino in correct position in grid
}