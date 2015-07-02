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

class Tetromino;

class Grid:public cocos2d::Sprite
{
public:
    CREATE_FUNC(Grid);
    
    void rotateActiveTetromino();
    
    void spawnTetromino(Tetromino* tetromino);
    
private:
    Tetromino* activeTetromino;

    bool init() override;
    void onEnter() override;
};

#endif /* defined(__Tetriminos__Grid__) */
