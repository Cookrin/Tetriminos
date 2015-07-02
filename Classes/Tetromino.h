//
//  Tetromino.h
//  Tetriminos
//
//  Created by Tingting Gu on 2015/07/01.
//
//

#ifndef __Tetriminos__Tetromino__
#define __Tetriminos__Tetromino__

#include "cocos2d.h"
#include "Constants.h"
#include "Coordinate.h"

class Tetromino : public cocos2d::Node
{
public:
    //
    static Tetromino* createWithType(TetrominoType type);
    
    void rotate(bool right);
    
private:
    //
    TetrominoType type;
    std::vector<std::vector<Coordinate>> rotations;
    cocos2d::Color3B color;
    std::vector<cocos2d::Sprite*> blocks;
    
    bool initWithType(TetrominoType type);
    
    int rotationIndex;

protected:
    
};

#endif /* defined(__Tetriminos__Tetromino__) */
