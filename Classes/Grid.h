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
    void dropActiveTetromino();
    
    int getScore();
    int getTotalLinesCleared();
    bool checkIfTopReached();
    std::vector<std::vector<cocos2d::Sprite*>> getBlocksLanded();
    
private:
    Tetromino* activeTetromino;
    Tetromino* ghostTetromino;
    std::vector<std::vector<cocos2d::Sprite*>> blocksLanded;
    
    int score;
    int totalLinesCleared;
    
    void updateScore(int linesCleared);
    
    bool init() override;
    void onEnter() override;
    cocos2d::Vec2 convertCoordinateToPosition(Coordinate coordinate);
    Coordinate activeTetrominoCoordinate;
    // a more general functional method, not depend on the state
        // less state you have the better
    bool checkIfTetrominoCollides(Tetromino* tetromino, Coordinate tetrominoCoordinate);
    
    void deactivateTetromino(Tetromino* tetromino, Coordinate tetrominoCoordinate);
    void placeTetrominoOnBoard(Tetromino *tetromino, Coordinate tetrominoCoordinate);
    Coordinate getTetrominoLandingCoordinate();
    
    void clearLines();
    
    void updateGhostTetrominoPosition();
};

#endif /* defined(__Tetriminos__Grid__) */