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
    
    this->activeTetromino = nullptr; // = 0
    
    // test JSON Data
    // Tetromino* test = Tetromino::createWithType(TetrominoType::I);
    
    // test Block
    // Tetromino* testBlock = Tetromino::createWithType(TetrominoType::Z);
    // this->addChild(testBlock);
    
    // test Block's retation
    // Tetromino* rotateBlock = Tetromino::createWithType(TetrominoType::I);
    // this->addChild(rotateBlock);
    // rotateBlock->rotate(true);
    
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
    
    // TODO: Place tetromino in correct position in grid
    
    this->activeTetromino->setAnchorPoint(Vec2(0.0f, 0.0f));
    int highestY = activeTetromino->getHighestYCoodinate();
    int width = activeTetromino->getWidthInBlocks();
    
    Coordinate spawnCoordinate = Coordinate((GRID_WIDTH /2) - (width / 2) - 1, GRID_HEIGHT- highestY-1);
    
    this->setActiveTetrominoCoordinate(spawnCoordinate);
    //Vec2 spawnPosition = this->convertCoordinateToPosition(spawnCoordinate);
    //this->activeTetromino->setPosition(spawnPosition);
    
    this->addChild(this->activeTetromino);
}

void Grid::step()
{
    Coordinate activeCoordinate = this->getActiveTetrominoCoordinate();
    
    Coordinate nextCoordinate = Coordinate(activeCoordinate.x, activeCoordinate.y-1);
    
    this->setActiveTetrominoCoordinate(nextCoordinate);
}

Coordinate Grid::getActiveTetrominoCoordinate()
{
    return this->activeTetrominoCoordinate;
}

Size Grid::getBlockSize()
{
    Size contentSize = this->getContentSize();
    Size blockSize = Size(contentSize.width / float(GRID_WIDTH), contentSize.height / float(GRID_HEIGHT));
    return blockSize;
}

Tetromino* Grid::getActiveTetromino()
{
    return this->activeTetromino;
}

#pragma mark -
#pragma mark Private Methods

Vec2 Grid::convertCoordinateToPosition(Coordinate coordinate)
{
    Size contentSize = this->getContentSize();
    
    float blockWidth = contentSize.width / float(GRID_WIDTH);
    float blockHeight = contentSize.height / float(GRID_HEIGHT);
    
    return Vec2(coordinate.x * blockWidth, coordinate.y * blockHeight);
}

void Grid::setActiveTetrominoCoordinate(Coordinate coordinate)
{
    if (activeTetromino)
    {
        activeTetrominoCoordinate = coordinate;
        
        activeTetromino->setPosition(this->convertCoordinateToPosition(activeTetrominoCoordinate));
    
    }
}