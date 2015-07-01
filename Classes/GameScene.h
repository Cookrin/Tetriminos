//
//  GameScene.h
//  Tetriminos
//
//  Created by Tingting Gu on 2015/07/01.
//
//

#ifndef __Tetriminos__GameScene__
#define __Tetriminos__GameScene__


#include <cocos2d.h>


// create the class GameScene
class GameScene : public cocos2d::Node
{
public:
    //
    CREATE_FUNC(GameScene);
    
private:
    // virtual bool init();
    bool init() override;
    //void onEnter() override;
};

#endif /* defined(__Tetriminos__GameScene__) */
