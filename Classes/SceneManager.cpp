//
//  SceneManager.cpp
//  Tetriminos
//
//  Created by Tingting Gu on 2015/07/01.
//
//

#include "SceneManager.h"
#include "GameScene.h"
#include "Lobby.h"


using namespace cocos2d;

static SceneManager* sharedSceneManager = nullptr;

SceneManager* SceneManager::getInstance()
{
    if (! sharedSceneManager)
    {
        sharedSceneManager = new (std::nothrow)
        SceneManager();
    }
    
    return sharedSceneManager;
}

#pragma mark -
#pragma mark LifeStyle

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

#pragma mark -
#pragma mark Public Methods

void SceneManager::enterGameScene(bool networked)
{
    Scene* scene = Scene::create();
    GameScene* gameScene = GameScene::create();
    
    scene->addChild(gameScene);
    
    Director::getInstance()->pushScene(scene);
}

void SceneManager::enterLobby(bool networked)
{
    Scene* scene = Scene::create();
    Lobby* lobby = Lobby::create();
    
    scene->addChild(lobby);
    
    Director::getInstance()->pushScene(scene);
}

