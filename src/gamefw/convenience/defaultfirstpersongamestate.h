#ifndef DEFAULTFIRSTPERSONGAMESTATE_H
#define DEFAULTFIRSTPERSONGAMESTATE_H

#include "../gamefw.h"
#include "defaultfirstpersoncontroller.h"

class DefaultFirstPersonGameState
:
public gamefw::IGameState
{
public:
    /**
     * @brief Constructs a game state for first person camera view.
     *
     * @param parent The main game class. Needed for it's renderer and main window.
     * @param camera The Entity set to the controlled view.
     **/
    DefaultFirstPersonGameState(Game* parent, shared_ptr< Entity > controllable);
    
    virtual ~DefaultFirstPersonGameState();
    
    /**
     * @brief Changes the controlled entity for the gamestate.
     *
     * @param controllable New Entity to control.
     */
    virtual void changeControlledEntity(shared_ptr< Entity > controllable);

    UpdateStatus update();

private:
    gamefw::Game* m_parent;
    DefaultFirstPersonController m_controller;
    int m_window_middle_x;
    int m_window_middle_y;
};

#endif // DEFAULTFIRSTPERSONGAMESTATE_H

