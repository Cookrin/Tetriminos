//
//  PreviewGrid.h
//  Tetriminos
//
//  Created by Tingting Gu on 2015/07/10.
//
//

#ifndef __Tetriminos__PreviewGrid__
#define __Tetriminos__PreviewGrid__

#include "cocos2d.h"
#include "JSONPacker.h"


class PreviewGrid : public cocos2d::Sprite
{
public:
    CREATE_FUNC(PreviewGrid);
    void setState(JSONPacker::GameState state);

private:
    bool init() override;
};

#endif /* defined(__Tetriminos__PreviewGrid__) */
