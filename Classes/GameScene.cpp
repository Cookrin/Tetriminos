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
#include "UIConstants.h"
#include "JSONPacker.h"
#include "NetworkingWrapper.h"
#include "PreviewGrid.h"

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
    
    this->totalScore = 0;

    this->stepInterval = INITIAL_STEP_INTERVAL;
    
    this->active = false;
    
    this->timeLeft = TIME_PER_GAME;
    
    this->networkedSession = false;
    this->gameIsOver = false;
    
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
    
    if (this->networkedSession)
    {
        this->grid->setAnchorPoint(Vec2(0.0f, 0.0f));
        this->grid->setPosition(Vec2(0.0f, 0.0f));
        
        this->previewGrid = PreviewGrid::create();
        this->previewGrid->setAnchorPoint(Vec2(1.0f, 1.0f));
        this->previewGrid->setPosition(Vec2(visibleSize.width, visibleSize.height));
        this->previewGrid->setScale(0.3f);
        this->addChild(previewGrid);
    }
    
    this->setupUI();
    this->setGameActive(true);
    this->setupTouchHanding();
}

void GameScene::gameOver()
{
    this->gameIsOver = true;
    this->setGameActive(false);
    
    if (this->networkedSession)
    {
        this->sendGameStateOverNetwork();
    }

    std::string scoreString = StringUtils::toString(totalScore);
    std::string messageContent = "Your score is " + scoreString + "!";
    
    MessageBox(messageContent.c_str(), "Game Over");
    
    SceneManager::getInstance()->enterLobby();
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
    
    //setup scoreLabel
    this->scoreLabel = ui::Text::create("0", FONT_NAME, FONT_SIZE);
    this->scoreLabel->setAnchorPoint(Vec2(0.5f, 1.0f));
    this->scoreLabel->setPosition(Vec2(visibleSize.width*0.5f, visibleSize.height * 0.95f));
    this->scoreLabel->setColor(LABEL_COLOR);
    this->addChild(scoreLabel);
    
    //setup timeLeftLabel
    this->timeLeftLabel = ui::Text::create("0", FONT_NAME, FONT_SIZE);
    this->timeLeftLabel->setAnchorPoint(Vec2(0.5f, 1.0f));
    this->timeLeftLabel->setPosition(this->scoreLabel->getPosition() - Vec2(0.0f, FONT_SIZE * 1.5f));
    this->timeLeftLabel->setColor(LABEL_COLOR);
    this->addChild(timeLeftLabel);
}

void GameScene::backButtonPressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    if (eEventType == ui::Widget::TouchEventType::ENDED)
    {
        SceneManager::getInstance()->enterLobby();
    };
    
}

void GameScene::updateScoreLabel(int score)
{
    std::string scoreString = StringUtils::toString(score);
    this->scoreLabel->setString(scoreString);
}

void GameScene::setTimeLeft(float time)
{
    this->timeLeft = time;
    std::string timeLeftString = StringUtils::format("%2.0f", time);
    this->timeLeftLabel->setString(timeLeftString);
}

void GameScene::updateStateFromScore()
{
    int newScore = this->grid->getScore();
    if (newScore > this->totalScore)
    {
        this->totalScore = newScore;
        this->updateScoreLabel(newScore);
        this->updateGameSpeed(this->totalScore);
    }
}

#pragma mark -
#pragma mark GameScene Public Methods


void GameScene::setNetworkedSession(bool networkedSession)
{
    this->networkedSession = networkedSession;
}

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
                    this->updateStateFromScore();
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
        this->schedule(CC_SCHEDULE_SELECTOR(GameScene::step), stepInterval);
        this->scheduleUpdate();
    } else {
        this->unschedule(CC_SCHEDULE_SELECTOR(GameScene::step));
        this->unscheduleUpdate();
    }
}

void GameScene::step(float dt) {
    if (this->grid->checkIfTopReached())
    {
        this->gameOver();
    }
    
    Tetromino* activeTetromino = grid->getActiveTetromino();
    if (! activeTetromino )
    {
        Tetromino* newTetromino = this->createRandomTetromino();
        this->grid->spawnTetromino(newTetromino);
    }
    else
    {
        this->grid->step();
        this->updateStateFromScore();
    }
    
    if (this->networkedSession)
    {
        this->sendGameStateOverNetwork();
    }
}

void GameScene::update(float dt)
{
    Node::update(dt);
    
    this->setTimeLeft(this->timeLeft -dt);

    if (timeLeft <= 0)
    {
        this->gameOver();
    }
}

void GameScene::updateGameSpeed(int score)
{
    int stepAcceleration = score / SCORE_TO_ACCELERATE;
    
    //int intervalDeduction = powf(ACCELERATTE_FACTOR, stepAcceleration) * INITIAL_STEP_INTERVAL;
    float intervalDeduction = INITIAL_STEP_INTERVAL * float(stepAcceleration) * ACCELERATTE_FACTOR;
    
    float newInterval = MAX((INITIAL_STEP_INTERVAL-intervalDeduction), SPEED_MAX);
    this->stepInterval = newInterval;
    
    this->unschedule(CC_SCHEDULE_SELECTOR(GameScene::step));
    this->schedule(CC_SCHEDULE_SELECTOR(GameScene::step), this->stepInterval);
}

void GameScene::receiveData(const void *data, unsigned long length)
{
    const char* cstr = reinterpret_cast<const char*>(data);
    std::string json = std::string(cstr, length);
    
    JSONPacker::GameState state = JSONPacker::unpackGameStateJSON(json);
    
    if (state.gameOver)
    {
        this->gameOver();
    }
    
    this->previewGrid->setState(state);
}

void GameScene::sendGameStateOverNetwork()
{
    JSONPacker::GameState state;
    
    state.name = NetworkingWrapper::getDeviceName();
    state.score = this->totalScore;
    state.gameOver = this->gameIsOver;
    
    std::vector<std::vector<Sprite*>> blocksLanded = this->grid->getBlocksLanded();
    
    for (int y = 0; y < blocksLanded.size(); ++y)
    {
        std::vector<Color3B> blocks (blocksLanded.size(), Color3B::WHITE);
        state.board.push_back(blocks);
        
        std::vector<Sprite*> column = blocksLanded[y];
        
        for (int x = 0; x < column.size(); ++x)
        {
            Sprite* block = column[x];
            
            if (block)
            {
                state.board[y][x] = block->getColor();
            }
        }
    }
    
    Tetromino* activeTetromino = this->grid->getActiveTetromino();
    
    if (activeTetromino)
    {
        // std::vector<Sprite*> blocks = activeTetromino->getBlocks();
        std::vector<Coordinate> coordinates = activeTetromino->getCurrentRotation();
        Coordinate tetrominoCoordinate = this->grid->getActiveTetrominoCoordinate();
        
        for (Coordinate blockCoordinate : coordinates)
        {
            Coordinate gridCoordinate = Coordinate::add(tetrominoCoordinate, blockCoordinate);
            
            if (gridCoordinate.x < GRID_WIDTH && gridCoordinate.y < GRID_HEIGHT)
            {
                state.board[gridCoordinate.y][gridCoordinate.x] = activeTetromino->getTetrominoColor();
            }
        }
    }
    
    std::string json = JSONPacker::packGameState(state);
    
    SceneManager::getInstance()->sendData(json.c_str(), json.length());
}

#pragma mark -
#pragma mark GameScene Utility Methods

Coordinate GameScene::convertPositionToCoodinate(Vec2 position)
{
    Size blockSize = this->grid->getBlockSize();    
    
    return Coordinate(position.x / blockSize.width, position.y / blockSize.height);
}