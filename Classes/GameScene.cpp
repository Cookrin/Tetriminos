//
//  GameScene.cpp
//  Tetriminos
//
//  Created by Tingting Gu on 2015/07/01.
//
//

#include "GameScene.h"
#include "SceneManager.h"
#include "Grid.h"
#include "TetrominoBag.h"
#include "Tetromino.h"
#include "Constants.h"



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
    
    this->tetrominoBag = std::unique_ptr<TetrominoBag>(new TetrominoBag());

    return true;
}

void GameScene::onEnter()
{
    Node::onEnter();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    // setup grid
    grid = Grid::create();
    grid->setAnchorPoint(Vec2(0.5f, 0.0f));
    grid->setPosition(Vec2(visibleSize.width* 0.5f, visibleSize.height * 0.0f));
    this->addChild(grid);
    
    Tetromino* randomTest = this->createRandomTetromino();
    grid->spawnTetromino(randomTest);
    
    this->setupUI();
    this->setupTouchHanding();
}

#pragma mark -
#pragma mark GameScene UI Methods

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

#pragma mark -
#pragma mark GameScene Public Methods

void GameScene::setupTouchHanding()
{
    auto touchListener = EventListenerTouchOneByOne::create();
    
    touchListener->onTouchBegan = [&](Touch* touch, Event* event)
    {
        return true;
    };
    
    touchListener->onTouchEnded = [&](Touch* touch, Event* event)
    {
        grid->rotateActiveTetromino();
        
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
}

Tetromino* GameScene::createRandomTetromino()
{
    TetrominoType tetrominoType = tetrominoBag->getTetromino();
    
    Tetromino* newTetromino = Tetromino::createWithType(tetrominoType);
    
    return newTetromino;
}