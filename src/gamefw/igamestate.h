
#ifndef IGAMESTATE_H
#define IGAMESTATE_H

namespace gamefw {

enum UpdateStatus {UPDATE_NORMAL, UPDATE_QUIT, UPDATE_MENU, UPDATE_PAUSE};

/**
 * @brief Encapsulates an independent state ie. main menu, pause or ingame.
 */
class IGameState
{
public:
    virtual ~IGameState() {};
    
    /**
    * @brief Processes input and sends the current state to the rendering pipeline.
    */
    virtual UpdateStatus update() = 0;
};

}

#endif // IGAMESTATE_H
