//
//  JSONPacker.h
//  Tetriminos
//
//  Created by Tingting Gu on 2015/07/01.
//
//

#ifndef __Tetriminos__JSONPacker__
#define __Tetriminos__JSONPacker__

#include "cocos2d.h"
#include "Coordinate.h"
#include "Constants.h"

namespace JSONPacker
{
    struct GameState
    {
        std::string name;
        bool gameOver;
        int score;
        std::vector<std::vector<cocos2d::Color3B>> board;
    };
    
    struct TetrominoState
    {
        cocos2d::Color3B color;

        std::vector<std::vector<Coordinate>> rotations;
    };
    
    TetrominoState unpackTetrominoJSON(std::string json, TetrominoType type);
    // std::string packLineData(const TetrominoState TetrominoState);
    
    GameState unpackGameStateJSON(std::string json);
    std::string packGameState(const GameState data);
}

#endif /* defined(__Tetriminos__JSONPacker__) */
