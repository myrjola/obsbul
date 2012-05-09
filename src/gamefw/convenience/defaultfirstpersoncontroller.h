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
    DefaultFirstPersonController(shared_ptr< Entity > controllable);

    virtual void keyPressed(sf::Event::KeyEvent& keyevent);

    virtual void keyReleased(sf::Event::KeyEvent& keyevent);

    virtual void mouseMoved(const int x, const int y);

    virtual void newMousePosition(const int width, const int height);

private:
    shared_ptr<Entity> m_controllable;

    int m_last_mouse_x;
    int m_last_mouse_y;
};

#endif // DEFAULTFIRSTPERSONCONTROLLER_H
