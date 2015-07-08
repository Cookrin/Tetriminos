//
//  GameScene.h
//  Tetriminos
//
//  Created by Tingting Gu on 2015/07/01.
//
//

#ifndef __Tetriminos__GameScene__
#define __Tetriminos__GameScene__


#include "cocos2d.h"
#include "CocosGUI.h"
#include "TetrominoBag.h"
#include "Coordinate.h"



class Grid;
class Tetromino;
class Coordinate;

// create the class GameScene
class GameScene : public cocos2d::Node
{
public:
    //
    CREATE_FUNC(GameScene);
     Coordinate convertPositionToCoodinate(cocos2d::Vec2 position);
    
private:
    //
    
protected:
    //
    Grid*grid;
    std::unique_ptr<TetrominoBag> tetrominoBag;

    
    bool init() override;
    void onEnter() override;

    void setupTouchHanding();
    
    void setupUI();
    void backButtonPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType eEventType);
    Tetromino* createRandomTetromino();
    
    void setGameActive(bool active);
    void step(float dt);
    bool active;
};

#endif /* defined(__Tetriminos__GameScene__) */
