//
//  SceneManager.h
//  Tetriminos
//
//  Created by Tingting Gu on 2015/07/01.
//
//

#ifndef __Tetriminos__SceneManager__
#define __Tetriminos__SceneManager__

#include <cocos2d.h>

class SceneManager
{
public:
    static SceneManager* getInstance();
    void enterGameScene(bool networked);
    void enterLobby(bool networked);
    
private:
    SceneManager();
    ~SceneManager();

 };
#endif /* defined(__Tetriminos__SceneManager__) */
