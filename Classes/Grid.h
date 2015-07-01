//
//  Grid.h
//  Tetriminos
//
//  Created by Tingting Gu on 2015/07/01.
//
//

#ifndef __Tetriminos__Grid__
#define __Tetriminos__Grid__

#include <stdio.h>

class Grid:public cocos2d::Sprite
{
public:
    CREATE_FUNC(Grid);
    
private:
    bool init() override;
};

#endif /* defined(__Tetriminos__Grid__) */
