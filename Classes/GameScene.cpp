//
//  GameScene.cpp
//  Tetriminos
//
//  Created by Tingting Gu on 2015/07/01.
//
//

#include "GameScene.h"
#include "SceneManager.h"

using namespace cocos2d;

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (! Node::init() )
    {
        return false;
    }

    LayerColor* background = LayerColor::create(Color4B(255, 255, 255, 255));
    
    this->addChild(background);

    return true;
}

void GameScene::onEnter()
{
    Node::onEnter();
    
    this->setupUI();
}

void GameScene::setupUI()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    //create the multiplayer button
    ui::Button* backButton = ui::Button::create();
    
    backButton->setAnchorPoint(Vec2(0.9f, 1.0f));
    backButton->setPosition(Vec2(visibleSize.width* 0.9f, visibleSize.height * 1.0f));
    backButton->loadTextures("backButton.png", "backButtonPressed.png");
    backButton->addTouchEventListener(CC_CALLBACK_2(GameScene::backButtonPressed, this));
    this->addChild(backButton);
}

void GameScene::backButtonPressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    if (eEventType == ui::Widget::TouchEventType::ENDED)
    {
        SceneManager::getInstance()->enterLobby(false);
    };
    
}
