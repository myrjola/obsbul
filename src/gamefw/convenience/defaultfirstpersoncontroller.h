#ifndef DEFAULTFIRSTPERSONCONTROLLER_H
#define DEFAULTFIRSTPERSONCONTROLLER_H

#include "../../common.h"
#include "../gamefw.h"

using namespace gamefw;

/**
 * @brief Controller providing simple first person camera control.
 **/
class DefaultFirstPersonController : public IController
{

public:
    /**
     * @brief Construct a controller for the given Entity.
     *
     * @param controllable ditto.
     **/
    DefaultFirstPersonController(const shared_ptr< Entity > controllable);

    virtual void keyPressed(sf::Event::KeyEvent& keyevent);

    virtual void keyReleased(sf::Event::KeyEvent& keyevent);

    virtual void mouseMoved(sf::Event::MouseMoveEvent event);   

private:
    shared_ptr<Entity> m_controllable;
};

#endif // DEFAULTFIRSTPERSONCONTROLLER_H
