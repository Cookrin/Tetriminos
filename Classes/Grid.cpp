 //
//  Grid.cpp
//  Tetriminos
//
//  Created by Tingting Gu on 2015/07/01.
//
//

#include "Grid.h"
#include "Tetromino.h"
#include "Constants.h"
#include "UIConstants.h"


using namespace cocos2d;


bool Grid::init()
{
    if (! Sprite::initWithFile("grid.png"))
    {
        return false;
    }
    
    this->activeTetromino = nullptr; // = 0
    
    // test JSON Data
    // Tetromino* test = Tetromino::createWithType(TetrominoType::I);
    
    // test Block
    // Tetromino* testBlock = Tetromino::createWithType(TetrominoType::Z);
    // this->addChild(testBlock);
    
    // test Block's retation
    // Tetromino* rotateBlock = Tetromino::createWithType(TetrominoType::I);
    // this->addChild(rotateBlock);
    // rotateBlock->rotate(true);
    
    this->activeTetrominoCoordinate = Coordinate();
    
    this->score = 0;
    this->totalLinesCleared = 0;
    
    for (int index = 0; index < GRID_HEIGHT; ++index)
    {
        std::vector<Sprite*> row(GRID_WIDTH, nullptr);
        blocksLanded.push_back(row);
    }
    return true;
}

void Grid::onEnter()
{
    Sprite::onEnter();
    
    //this->activeTetromino = Tetromino::createWithType(TetrominoType::L);
    //this->addChild(activeTetromino);
}

void Grid::rotateActiveTetromino()
{
    if (this->activeTetromino)
    {
        this->activeTetromino->rotate(true);
        
        //Check if collision, undo rotation
        
        if (this->checkIfTetrominoCollides(activeTetromino, activeTetrominoCoordinate))
        {
            this->activeTetromino->rotate(false);
        }
        else
        {
            ghostTetromino->rotate(true);
            this->updateGhostTetrominoPosition();
        }
    }
}

void Grid::spawnTetromino(Tetromino *tetromino)
{
    this->activeTetromino = tetromino;
    
    // TODO: Place tetromino in correct position in grid
    
    this->activeTetromino->setAnchorPoint(Vec2(0.0f, 0.0f));
    int highestY = activeTetromino->getHighestYCoodinate();
    int width = activeTetromino->getWidthInBlocks();
    
    Coordinate spawnCoordinate = Coordinate((GRID_WIDTH /2) - (width / 2) - 1, GRID_HEIGHT- highestY-1);
    
    this->setActiveTetrominoCoordinate(spawnCoordinate);
    
    Vec2 spawnPosition = this->convertCoordinateToPosition(spawnCoordinate);
    this->activeTetromino->setPosition(spawnPosition);
    this->addChild(this->activeTetromino);
    
    // add ghostTetromino
    TetrominoType type = tetromino->getTetrominoType();
    this->ghostTetromino = Tetromino::createWithType(type);
    
    this->ghostTetromino->setCascadeOpacityEnabled(true);
    
    this->ghostTetromino->setOpacity(GHOST_TETROMINO_OPACITY);
    this->updateGhostTetrominoPosition();
    
    this->addChild(ghostTetromino);
}

void Grid::step()
{
    if (activeTetromino)
    {
        Coordinate activeCoordinate = this->getActiveTetrominoCoordinate();
        
        Coordinate nextCoordinate = Coordinate(activeCoordinate.x, activeCoordinate.y-1);
        
        if (checkIfTetrominoCollides(activeTetromino, nextCoordinate))
        {
            this->deactivateTetromino(activeTetromino, activeCoordinate);
        }
        else
        {
            this->setActiveTetrominoCoordinate(nextCoordinate);
        }
    }
}

Coordinate Grid::getActiveTetrominoCoordinate()
{
    return this->activeTetrominoCoordinate;
}

Size Grid::getBlockSize()
{
    Size contentSize = this->getContentSize();
    Size blockSize = Size(contentSize.width / float(GRID_WIDTH), contentSize.height / float(GRID_HEIGHT));
    return blockSize;
}

Tetromino* Grid::getActiveTetromino()
{
    return this->activeTetromino;
}

void Grid::dropActiveTetromino()
{
    Coordinate landingCoordinate = this->getTetrominoLandingCoordinate();
    
    this->setActiveTetrominoCoordinate(landingCoordinate);
    this->deactivateTetromino(activeTetromino, activeTetrominoCoordinate);
}

int Grid::getScore()
{
    return this->score;
}

int Grid::getTotalLinesCleared()
{
    return this->totalLinesCleared;
}

std::vector<std::vector<cocos2d::Sprite*>> Grid::getBlocksLanded()
{
    return this->blocksLanded;
}


bool Grid::checkIfTopReached()
{
    for (Sprite* block : blocksLanded.back())
    {
        if (block)
        {
            return true;
        }
    }
    return false;
}

#pragma mark -
#pragma mark Private Methods

Vec2 Grid::convertCoordinateToPosition(Coordinate coordinate)
{
    Size contentSize = this->getContentSize();
    
    float blockWidth = contentSize.width / float(GRID_WIDTH);
    float blockHeight = contentSize.height / float(GRID_HEIGHT);
    
    return Vec2(coordinate.x * blockWidth, coordinate.y * blockHeight);
}

void Grid::setActiveTetrominoCoordinate(Coordinate coordinate)
{
    if (activeTetromino)
    {
        if (! this->checkIfTetrominoCollides(activeTetromino, coordinate))
        {
            activeTetrominoCoordinate = coordinate;
            
            activeTetromino->setPosition(this->convertCoordinateToPosition(activeTetrominoCoordinate));
        
            this->updateGhostTetrominoPosition();
        }
    }
}

bool Grid::checkIfTetrominoCollides(Tetromino* tetromino, Coordinate tetrominoCoordinate)
{
    int skirtStart = tetromino->getMinimumXCoordinate();
    std::vector<int> skirt = tetromino->getSkirt();
    
    for (int index= 0; index < skirt.size(); ++index)
    {
        Coordinate localCoordinate = Coordinate(index + skirtStart, skirt[index]);
        Coordinate gridCoordinate = Coordinate::add(tetrominoCoordinate, localCoordinate);
        
        if (gridCoordinate.x < 0 || gridCoordinate.y < 0 || gridCoordinate.x >= GRID_WIDTH || gridCoordinate.y >= GRID_HEIGHT)
        {
            return true;
        }
        
        if (blocksLanded[gridCoordinate.y][gridCoordinate.x])
        {
            return true;
        }
    }
    return false;
}

void Grid::deactivateTetromino(Tetromino* tetromino, Coordinate tetrominoCoordinate)
{
    this->placeTetrominoOnBoard(tetromino, tetrominoCoordinate);
    
    this->activeTetromino->removeFromParent();
    this->activeTetromino = nullptr;
    
    this->ghostTetromino->removeFromParent();
    this->ghostTetromino = nullptr;
    
    this->clearLines();
}

void Grid::placeTetrominoOnBoard(Tetromino *tetromino, Coordinate tetrominoCoordinate)
{
    std::vector<Sprite*> blocks = tetromino->getBlocks();
    std::vector<Coordinate> coordinates = tetromino->getCurrentRotation();
    
    for (int index = 0; index < TETROMINO_BLOCK_NUMBER; ++index)
    {
        // get block's tetromino coordinate
        Coordinate coordinate = coordinates[index];
        
        // convert to a grid coordinate
        Coordinate globalCoordinate = Coordinate::add(tetrominoCoordinate, coordinate);
        
        // get the block sprite
        Sprite* block = blocks[index];
        
        // get the block's current position in the grid space
        Vec2 gridPosition = block->getPosition() + tetromino->getPosition();
        
        // remove the block from the tetromino
        block->retain();
        block->removeFromParent();
        
        // add the block to grid(this)
        this->addChild(block);
        block->release();
        
        // set the block sprite position(in terms of grid)
        block->setPosition(gridPosition);
        
        // add the block to blockland
        blocksLanded[globalCoordinate.y][globalCoordinate.x] = block;
    }
}

Coordinate Grid::getTetrominoLandingCoordinate()
{
    bool collide = false;
    
    Coordinate landingCoordinate = this->getActiveTetrominoCoordinate();
    
    while(!collide)
    {
        landingCoordinate.y--;

        if ( this->checkIfTetrominoCollides(activeTetromino, landingCoordinate) )
        {
        landingCoordinate.y++;
        collide = true;
        }
    }
    return landingCoordinate;
}

void Grid::clearLines()
{
    int linesCleared = 0;
    
    for (int y = 0; y < GRID_HEIGHT; ++y)
    {
        // check if all the blocks in a row are filled
        bool fullLine = true;
        
        std::vector<Sprite*> row = blocksLanded[y];
        
        for (int x = 0; x < GRID_WIDTH; ++x)
        {
            if (!row[x])
            {
                fullLine = false;
                break;
            }
        }
    
        // clear the line if filled
        if (fullLine)
        {
            // remove the block sprites from grid and blockLanded
            for (Sprite* block : row)
            {
                block->removeFromParent();
            }
            blocksLanded.erase(blocksLanded.begin() + y);
            // move blocks in all line
            std::vector<std::vector<Sprite*>> rowToMoveDown(blocksLanded.begin() + y, blocksLanded.end());
            
            for (std::vector<Sprite*> rowAove : rowToMoveDown)
            {
                for (Sprite* block : rowAove)
                {
                    if (block)
                    {
                        block->setPositionY(block->getPosition().y - block->getContentSize().height);
                    }
                }
            }
            
            linesCleared++;
            
            std::vector<Sprite*> newRow(GRID_WIDTH, nullptr);
            blocksLanded.push_back(newRow);
            
            y--;
        }
    }
    
    this->totalLinesCleared += linesCleared;
    this->updateScore(linesCleared);
}

void Grid::updateGhostTetrominoPosition()
{
    if (ghostTetromino)
    {
        Coordinate landingCoordinate = this->getTetrominoLandingCoordinate();
        ghostTetromino->setPosition(this->convertCoordinateToPosition(landingCoordinate));
    }
}

void Grid::updateScore(int linesCleared)
{
    int scoreToAdd = linesCleared;
    
    if (linesCleared == 4)
    {
        scoreToAdd = 5;
    }
    
    this->score += scoreToAdd;
}