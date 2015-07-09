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
#include "Coordinate.h"
#include <time.h>


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

    this->active = false;
    
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
    this->setGameActive(true);
    this->setupTouchHanding();
}

#pragma mark -
#pragma mark GameScene UI Methods

void GameScene::setupUI()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    //create the multiplayer button
    ui::Button* backButton = ui::Button::create();
    
    backButton->setAnchorPoint(Vec2(0.1f, 1.0f));
    backButton->setPosition(Vec2(visibleSize.width* 0.01f, visibleSize.height * 0.99f));
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
    
    // define a static variable, so we can access the value and use it outside the function
    static Vec2 firstTouchPos;
    static Vec2 lastTouchPos;
    static bool allowRotate;
    static std::clock_t touchStartedTime;
    
    touchListener->onTouchBegan = [&](Touch* touch, Event* event)
    {
        firstTouchPos = this->convertTouchToNodeSpace(touch);
        lastTouchPos = firstTouchPos;
        allowRotate = true;
        touchStartedTime = clock();
        return true;
    };
    
    touchListener->onTouchMoved = [&](Touch* touch, Event* event)
    {
        // current touch position
        Vec2 touchPos = this->convertTouchToNodeSpace(touch);
        
        Vec2 difference = touchPos - lastTouchPos;
        
        Tetromino* activeTetromino = grid->getActiveTetromino();
        
        if (activeTetromino)
        {
            // Don't use: Coordinate touchCoordinate = this->convertPositionToCoodinate(touchPos);
            Coordinate differenceCoordinate = this->convertPositionToCoodinate(difference);
            Coordinate activeTetrominoCoordinate = grid->getActiveTetrominoCoordinate();
            
            //int heightDifference = this->convertPositionToCoodinate(firstTouchPos - touchPos).y;
            
            if (differenceCoordinate.y <= -1)
            {
                //CCLOG("MOVING DOWN!");
                Coordinate newTetrominoCoordinate = Coordinate(activeTetrominoCoordinate.x, activeTetrominoCoordinate.y-1);
                grid->setActiveTetrominoCoordinate(newTetrominoCoordinate);
                lastTouchPos = touchPos;
            }
            
            // either move it down or move it left and right
            else if (abs(differenceCoordinate.x)>=1)
            {
                Coordinate newTetrominoCoordinate;
                bool movingRight = (differenceCoordinate.x > 0);
                newTetrominoCoordinate = Coordinate(activeTetrominoCoordinate.x + (movingRight ? 1: -1),activeTetrominoCoordinate.y);
                grid->setActiveTetrominoCoordinate(newTetrominoCoordinate);
                allowRotate = false;
                lastTouchPos = touchPos;
            }
        }
    };
    
    touchListener->onTouchEnded = [&](Touch* touch, Event* event)
    {
        if (this->grid->getActiveTetromino())
        {
        
            Vec2 touchEndPos = this->convertTouchToNodeSpace(touch);
            float distance = touchEndPos.distance(firstTouchPos);
            
            Size blockSize = this->grid->getBlockSize();
            
            // allowRotate
            if (distance < blockSize.width && allowRotate)
            {
                grid->rotateActiveTetromino();
            }

            else {
                std::clock_t clockDifference = (float) (clock() - touchStartedTime) / CLOCKS_PER_SEC;
                Vec2 totalDifference = touchEndPos - lastTouchPos; // maybe firstTouchPos
                
                float velocity = fabsf(totalDifference.y / clockDifference);
                
                if (clockDifference <= 0)
                {
                    return;
                }
                
                if (velocity > DROP_VELOCITY)
                {
                    CCLOG("DROP VELOCITY was %f", velocity);
                    this->grid->dropActiveTetromino();
                }
            }
        }
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

Tetromino* GameScene::createRandomTetromino()
{
    TetrominoType tetrominoType = tetrominoBag->getTetromino();
    Tetromino* newTetromino = Tetromino::createWithType(tetrominoType);
    
    return newTetromino;
}

void GameScene::setGameActive(bool active) {
    this->active = active;
    if (active) {
        this->schedule(CC_SCHEDULE_SELECTOR(GameScene::step), INITIAL_STEP_INTERVAL);
    } else {
        this->unschedule(CC_SCHEDULE_SELECTOR(GameScene::step));
    }
}

void GameScene::step(float dt) {
   
    Tetromino* activeTetromino = grid->getActiveTetromino();
    if (! activeTetromino )
    {
        Tetromino* newTetromino = this->createRandomTetromino();
        this->grid->spawnTetromino(newTetromino);
    }
    else
    {
    this->grid->step();
    }
}

#pragma mark -
#pragma mark GameScene Utility Methods

Coordinate GameScene::convertPositionToCoodinate(Vec2 position)
{
    Size blockSize = this->grid->getBlockSize();    
    
    return Coordinate(position.x / blockSize.width, position.y / blockSize.height);
}