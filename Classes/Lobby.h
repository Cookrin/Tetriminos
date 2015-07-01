//
//  Lobby.h
//  Tetriminos
//
//  Created by Tingting Gu on 2015/07/01.
//
//

#ifndef __Tetriminos__Lobby__
#define __Tetriminos__Lobby__

#include <cocos2d.h>


// create the class Lobby
class Lobby : public cocos2d::Node
{
public:
    //
    CREATE_FUNC(Lobby);
    
private:
    // virtual bool init();
    bool init() override;
    //void onEnter() override;
};

#endif /* defined(__Tetriminos__Lobby__) */
