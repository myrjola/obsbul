#ifndef DEFAULTFIRSTPERSONCONTROLLER_H
#define DEFAULTFIRSTPERSONCONTROLLER_H

#include "../../common.h"
#include "../gamefw.h"

using namespace gamefw;

class DefaultFirstPersonController : public IController
{

public:
    DefaultFirstPersonController(shared_ptr<Entity> controllable);

    virtual void keyPressed(sf::Key::Code keycode);

    virtual void keyReleased(sf::Key::Code keycode);

    virtual void mouseMoved(sf::Event::MouseMoveEvent event);   

private:
    shared_ptr<Entity> m_controllable;
};

#endif // DEFAULTFIRSTPERSONCONTROLLER_H
