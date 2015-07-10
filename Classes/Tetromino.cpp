//
//  Tetromino.cpp
//  Tetriminos
//
//  Created by Tingting Gu on 2015/07/01.
//
//

#include "Tetromino.h"
#include "JSONPacker.h"
#include "Coordinate.h"

#define GRID_SIZE 4

using namespace cocos2d;

Tetromino* Tetromino::createWithType(TetrominoType type)
{
    Tetromino* tetromino = new (std::nothrow) Tetromino();
    
    if (tetromino && tetromino->initWithType(type))
    {
        tetromino->autorelease();
        
        return tetromino;
    }
    
    CC_SAFE_DELETE(tetromino);
    return nullptr;
}

#pragma mark -
#pragma mark Tetromino

bool Tetromino::initWithType(TetrominoType type)
{

    if (! Node::init())
    {
        return false;
    }
    
    this->type = type;
    
    std::string jsonString = FileUtils::getInstance()->getStringFromFile("tetrominos.json");
    // we don't need to set the fullPath
    // std::string jsonString = FileUtils::getInstance()->getStringFromFile(fullPath);
    
    JSONPacker::TetrominoState tetrominoState = JSONPacker::unpackTetrominoJSON(jsonString, type);
    
    this->color = tetrominoState.color;
    this->rotations = tetrominoState.rotations;
    
    this->blocks = std::vector<Sprite*>();
    this->blocks.reserve(TETROMINO_BLOCK_NUMBER);
    
    this->rotationIndex = 0;
    
    Sprite* dummyBlock = Sprite::create("block.png");
    Size dummySize = dummyBlock->getContentSize();
    
    float gridSizeF = float(GRID_SIZE);
    
    this->setContentSize(Size(dummySize.width * gridSizeF, dummySize.height * gridSizeF));
    
    auto coordinates = rotations[rotationIndex];
    
    for (Coordinate coordinate : coordinates)
    {
        Sprite* block = Sprite::create("block.png");
        block->setColor(this->color);
        
        block->setAnchorPoint(Vec2(0.0f,0.0f));
        block->setPosition(Vec2(coordinate.x  * dummySize.width, coordinate.y * dummySize.height));
         
        this->addChild(block);
        blocks.push_back(block);
    }
    
    return true;
}

#pragma mark-
#pragma mark public Methods

void Tetromino::rotate(bool right)
{
    //first set the rotation index
    if (right)
    {
        rotationIndex++;
    }
    else
    {
        rotationIndex--;
    }
    
    if (rotationIndex >=(int) rotations.size())
    {
        rotationIndex = 0;
    }
    else if (rotationIndex < 0)
    {
        rotationIndex = (int) rotations.size() - 1;
    }
    
    auto coordinates = rotations[rotationIndex];

    // then move block positions
    for (int index = 0; index < GRID_SIZE; ++index)
    {
        Sprite* block = blocks[index];
        Coordinate coordinate = coordinates[index];
        Size blockSize = block->getContentSize();
        
        block->setPosition(Vec2(coordinate.x  * blockSize.width, coordinate.y * blockSize.height));
    }
}

int Tetromino::getHighestYCoodinate()
{
    auto coordinates = rotations[rotationIndex];

    int highest = 0;
    
    /*
    
    for (int index = 0; index < GRID_SIZE; ++index)
    {
        Coordinate coordinate = coordinates[index];
        
        
        auto YCoodinate = coordinate.y;
        
        if (YCoodinate > highest)
        {
            highest = YCoodinate;
        }
    }
    */
    
    for (Coordinate coordinate : coordinates) {
        highest = MAX(highest, coordinate.y);
    }
    return highest;
}

int Tetromino::getWidthInBlocks()
{
    auto coordinates = rotations[rotationIndex];
    
    
    int rightMost = 0;
    
    int leftMost = GRID_SIZE;
    
    for (int index = 0; index < GRID_SIZE; ++index)
    {
        Coordinate coordinate = coordinates[index];

        if (coordinate.x < leftMost)
        {
            leftMost = coordinate.x;
        }
        
        if (coordinate.x > rightMost)
        {
            rightMost = coordinate.x;
        }

    }

    return rightMost - leftMost + 1;
}

int Tetromino::getMinimumXCoordinate()
{
    auto coordinates = rotations[rotationIndex];
    
    int leftMost = GRID_SIZE;
    
    for (Coordinate coordinate : coordinates)
    {
        if (coordinate.x < leftMost)
        {
            leftMost = coordinate.x;
        }
        
    }
    return leftMost;
}

std::vector<int> Tetromino::getSkirt()
{
    int width = this->getWidthInBlocks();
    int skirtStart = this->getMinimumXCoordinate();
    
    std::vector<int> skirt = std::vector<int>(width, GRID_SIZE - 1);
    
    auto coordinates = this->getCurrentRotation();

    for (Coordinate coordinate : coordinates)
    {
        int x = coordinate.x - skirtStart;
        int y = coordinate.y;
        int skirtY = skirt[x];
        
        if (y < skirtY)
        {
            skirt[x] = coordinate.y;
        }
    }
    return skirt;
}

std::vector<Sprite*> Tetromino::getBlocks()
{
    return this->blocks;
}

std::vector<Coordinate> Tetromino::getCurrentRotation()
{
    return rotations[rotationIndex];
}

TetrominoType Tetromino::getTetrominoType()
{
    return this->type;
}

Color3B Tetromino::getTetrominoColor()
{
    return this->color;
}