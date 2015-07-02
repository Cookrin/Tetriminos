//
//  TetrominoBag.h
//  Tetriminos
//
//  Created by Tingting Gu on 2015/07/02.
//
//

#ifndef __Tetriminos__TetrominoBag__
#define __Tetriminos__TetrominoBag__

#include "cocos2d.h"
#include "Constants.h"

class TetrominoBag
{
public:
    TetrominoBag();
    
    TetrominoType getTetromino();

private:
    std::vector<TetrominoType> bag;
    std::default_random_engine randomGenerator;
};

#endif /* defined(__Tetriminos__TetrominoBag__) */
