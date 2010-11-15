#ifndef DEFAULTFIRSTPERSONCONTROLLER_H
#define DEFAULTFIRSTPERSONCONTROLLER_H

#include "../../common.h"
#include "../gamefw.h"

using namespace gamefw;

class DefaultFirstPersonController : public IController
{

public:
    DefaultFirstPersonController(const shared_ptr< Entity > controllable,
                                 const uint display_width,
                                 const uint display_height);

    virtual void keyPressed(sf::Event::KeyEvent& keyevent);

    virtual void keyReleased(sf::Event::KeyEvent& keyevent);

    virtual void mouseMoved(sf::Event::MouseMoveEvent event);   

private:
    shared_ptr<Entity> m_controllable;
};

#endif // DEFAULTFIRSTPERSONCONTROLLER_H
