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
    
private:
    //
    
protected:
    //
    Grid*grid;
    std::unique_ptr<TetrominoBag> tetrominoBag;
    bool active;
    int totalScore;
    float stepInterval;
    float timeLeft;
    
    //Lifecycle
    bool init() override;
    void onEnter() override;
    void setupTouchHanding();
    void update(float dt) override;
    void backButtonPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType eEventType);
    
    //Game Logic
    Tetromino* createRandomTetromino();
    void setGameActive(bool active);
    void step(float dt);
    void updateStateFromScore();
    void updateGameSpeed(int score);
    void gameOver();
    void setTimeLeft(float time);
    
    //Utility
    Coordinate convertPositionToCoodinate(cocos2d::Vec2 position);

    //UI
    void setupUI();
    void updateScoreLabel(int score);
    cocos2d::ui::Text* scoreLabel;
    cocos2d::ui::Text* timeLeftLabel;
};

#endif /* defined(__Tetriminos__GameScene__) */
