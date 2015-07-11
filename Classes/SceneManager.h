//
//  SceneManager.h
//  Tetriminos
//
//  Created by Tingting Gu on 2015/07/01.
//
//

#ifndef __Tetriminos__SceneManager__
#define __Tetriminos__SceneManager__

#include "NetworkingWrapper.h"
class GameScene;

class SceneManager : public NetworkingDelegate
{
public:
    static SceneManager* getInstance();
    
    void enterGameScene(bool networked);
    void enterLobby();
    void showPeerList();
    void receiveMultiplayerInvitations();
    void sendData(const void* data, unsigned long length);
    
private:
    std::unique_ptr<NetworkingWrapper> networkingWrapper;
    GameScene* gameScene;
    SceneManager();
    ~SceneManager();
    
    void receivedData(const void* data, unsigned long length);
    void stateChanged(ConnectionState state);
 };
#endif /* defined(__Tetriminos__SceneManager__) */
