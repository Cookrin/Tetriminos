//
//  JSONPacker.cpp
//  Tetriminos
//
//  Created by Tingting Gu on 2015/07/01.
//
//

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
}