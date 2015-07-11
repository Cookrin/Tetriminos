//
//  JSONPacker.cpp
//  Tetriminos
//
//  Created by Tingting Gu on 2015/07/01.
//
//

#include "cocos2d.h"
#include "JSONPacker.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "Constants.h"

using namespace cocos2d;

namespace JSONPacker
{
    TetrominoState unpackTetrominoJSON(std::string json, TetrominoType type)
    {
        rapidjson::Document document;
        document.Parse<0>(json.c_str());
        
        const char* typeStr;
        
        switch (type)
        {
        case TetrominoType::I: typeStr = "I"; break;
        case TetrominoType::O: typeStr = "O"; break;
        case TetrominoType::T: typeStr = "T"; break;
        case TetrominoType::S: typeStr = "S"; break;
        case TetrominoType::Z: typeStr = "Z"; break;
        case TetrominoType::J: typeStr = "J"; break;
        case TetrominoType::L: typeStr = "L"; break;
        }
        // ampersand or and symbol "&"
        rapidjson::Value& tetrominoDoc = document[typeStr];
        rapidjson::Value& colorDoc = tetrominoDoc["color"];
        int r = colorDoc["r"].GetInt();
        int g = colorDoc["g"].GetInt();
        int b = colorDoc["b"].GetInt();
        Color3B color = Color3B(r,g,b);
        
        std::vector<std::vector<Coordinate>> rotations;
        rapidjson::Value& rotationDoc = tetrominoDoc["rotations"];
        
        for (rapidjson::SizeType rotationIndex = 0; rotationIndex < rotationDoc.Size(); ++rotationIndex)
        {
            std::vector<Coordinate> rotation;
            
            rapidjson::Value& coordinateDoc = rotationDoc[rotationIndex];
            
            for (rapidjson::SizeType coordinateIndex = 0; coordinateIndex < coordinateDoc.Size(); ++coordinateIndex)
            {
                int x = coordinateDoc[coordinateIndex]["x"].GetInt();
                int y = coordinateDoc[coordinateIndex]["y"].GetInt();
                Coordinate coordinate = Coordinate(x, y);
                rotation.push_back(coordinate);
            }
            
            rotations.push_back(rotation);
        }
        // struct is default public
        TetrominoState tetrominoState;
        tetrominoState.color = color;
        tetrominoState.rotations = rotations;
        
        return tetrominoState;
        

    }
    
    std::string packGameState(const GameState gameState)
    {
        rapidjson::Document document;
        document.SetObject();
        
        document.AddMember("score", gameState.score, document.GetAllocator());
        document.AddMember("name", gameState.name.c_str(), document.GetAllocator());
        document.AddMember("gameOver", gameState.gameOver, document.GetAllocator());
        
        rapidjson::Value columns(rapidjson::kArrayType);
        for (int column = 0; column < gameState.board.size(); ++column)
        {
            rapidjson::Value blocks(rapidjson::kArrayType);
            
            for (int index = 0; index < gameState.board[column].size(); ++index)
            {
                Color3B color = gameState.board[column][index];
                
                rapidjson::Value colorJson(rapidjson::kObjectType);
                colorJson.AddMember("r", color.r, document.GetAllocator());
                colorJson.AddMember("g", color.g, document.GetAllocator());
                colorJson.AddMember("b", color.b, document.GetAllocator());
                
                blocks.PushBack(colorJson, document.GetAllocator());
            }
            
            columns.PushBack(blocks, document.GetAllocator());
        }
        document.AddMember("board", columns, document.GetAllocator());
        
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        document.Accept(writer);
        
        std::string returnString(buffer.GetString(), buffer.Size());
        
        return returnString;
    }
    GameState unpackGameStateJSON(std::string json)
    {
        rapidjson::Document document;
        document.Parse<0>(json.c_str());
        
        GameState gameState;
        
        gameState.score = document["score"].GetInt();
        gameState.name = document["name"].GetString();
        gameState.gameOver = document["gameOver"].GetBool();
        
        rapidjson::Value& columns = document["board"];
        
        // convert Color3B objects in the grid to JSON objects;
        
        for (rapidjson::SizeType column = 0; column < columns.Size(); ++ column)
        {
            rapidjson::Value& blocksJson = columns[column];
            
            std::vector<Color3B> blocks;
            
            for (rapidjson::SizeType index = 0; index < blocksJson.Size(); ++index)
            {
                rapidjson::Value& block = blocksJson[index];
                int r = block["r"].GetInt();
                int g = block["g"].GetInt();
                int b = block["b"].GetInt();
                
                Color3B color = Color3B(r, g, b);
                
                blocks.push_back(color);
            }
            
            gameState.board.push_back(blocks);
        }
        return gameState;
    }
}