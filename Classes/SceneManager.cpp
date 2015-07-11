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
    gameScene = nullptr;
    this->networkingWrapper = std::unique_ptr<NetworkingWrapper>(new NetworkingWrapper());
    this->networkingWrapper->setDelegate(this);
}

SceneManager::~SceneManager()
{
}

#pragma mark -
#pragma mark Public Methods

void SceneManager::enterGameScene(bool networked)
{
    Scene* scene = Scene::create();
    this->gameScene = GameScene::create();
    this->gameScene->setNetworkedSession(networked);
    
    scene->addChild(gameScene);
    
    Director::getInstance()->pushScene(scene);
}

void SceneManager::enterLobby()
{
    if (gameScene)
    {
        Director::getInstance()->popScene();
        gameScene = nullptr;
        networkingWrapper->disconnect();
    }
}

#pragma mark -
#pragma mark NetworkingWrapper Methods

void SceneManager::showPeerList()
{
    networkingWrapper->showPeerList();
}

void SceneManager::receiveMultiplayerInvitations()
{
    networkingWrapper->startAdvertisingAvailability();
}

void SceneManager::receivedData(const void *data, unsigned long length)
{
    if (gameScene)
    {
        gameScene->receiveData(data, length);
    }
}

void SceneManager::stateChanged(ConnectionState state)
{
    switch (state)
    {
        case ConnectionState::CONNECTING:
            CCLOG("Connecting");
            break;
        
        case ConnectionState::NOT_CONNECTED:
            CCLOG("Not Connected");
            break;
        
        case ConnectionState::CONNECTED:
            CCLOG("Connected");
            if (!gameScene)
            {
                this->networkingWrapper->stopAdvertisingAvailability();
                this->enterGameScene(true);
            }
    }
}

void SceneManager::sendData(const void *data, unsigned long length)
{
    networkingWrapper->sendData(data, length);
}
