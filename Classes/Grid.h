//
//  Grid.h
//  Tetriminos
//
//  Created by Tingting Gu on 2015/07/01.
//
//

#ifndef __Tetriminos__Grid__
#define __Tetriminos__Grid__

#include "cocos2d.h"
#include "Tetromino.h"
#include "Coordinate.h"


class Tetromino;

class Grid:public cocos2d::Sprite
{
public:
    CREATE_FUNC(Grid);
    
    void rotateActiveTetromino();
    
    void spawnTetromino(Tetromino* tetromino);
    
    void step();
    
    void setActiveTetrominoCoordinate(Coordinate coordinate);
    Coordinate getActiveTetrominoCoordinate();
    
    cocos2d::Size getBlockSize();
    Tetromino* getActiveTetromino();
    
private:
    Tetromino* activeTetromino;

    bool init() override;
    void onEnter() override;
    cocos2d::Vec2 convertCoordinateToPosition(Coordinate coordinate);
    Coordinate activeTetrominoCoordinate;
};

#endif /* defined(__Tetriminos__Grid__) */