//
//  Lobby.cpp
//  Tetriminos
//
//  Created by Tingting Gu on 2015/07/01.
//
//

#include "Lobby.h"
#include "GameScene.h"
#include "SceneManager.h"

using namespace cocos2d;

bool Lobby::init()
{
    if (! Node::init())
    {
        return false;
    }
    
    LayerColor* background = LayerColor::create(Color4B(255, 255, 255, 255));
    
    this->addChild(background);
    
    return true;
}

// define the onEnter
void Lobby::onEnter()
{
    Node::onEnter();
    
    this->setupUI();
}

void Lobby::setupUI()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    // create logo image
    Sprite* logo = Sprite::create("logo.png");
    logo->setAnchorPoint(Vec2(0.5f, 0.5f));
    logo->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.75f));
    this->addChild(logo);
    
    // create the single player button
    ui::Button* singlePlayerButton = ui::Button::create();
    singlePlayerButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    singlePlayerButton->setPosition(Vec2(visibleSize.width / 2.0f, visibleSize.height * 0.4f));
    singlePlayerButton->loadTextures("singlePlayerButton.png", "singlePlayerButtonPressed.png");
    singlePlayerButton->addTouchEventListener(CC_CALLBACK_2(Lobby::singlePlayerPressed, this));
    this->addChild(singlePlayerButton);
    
    //create the multiplayer button
    ui::Button* multiplayerButton = ui::Button::create();
    multiplayerButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    multiplayerButton->setPosition(Vec2(visibleSize.width / 2.0f, visibleSize.height * 0.25f));
    multiplayerButton->loadTextures("multiplayerButton.png", "multiplayerButtonPressed.png");
    multiplayerButton->addTouchEventListener(CC_CALLBACK_2(Lobby::multiplayerPressed, this));
    this->addChild(multiplayerButton);
}

void Lobby::singlePlayerPressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    if (eEventType == ui::Widget::TouchEventType::ENDED)
    {
        SceneManager::getInstance()->enterGameScene(false);
        
    };

}

void Lobby::multiplayerPressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    
}