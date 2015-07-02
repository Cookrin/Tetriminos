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
    
    this->blocks = std::vector<Sprite*>(4);
    
    Sprite* dummyBlock = Sprite::create("block.png");
    Size dummySize = dummyBlock->getContentSize();
    
    float gridSizeF = float(GRID_SIZE);
    
    this->setContentSize(Size(dummySize.width * gridSizeF, dummySize.height * gridSizeF));
    
    auto coordinates = rotations[0];
    
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

