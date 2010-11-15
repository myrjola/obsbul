#ifndef ICONTROLLER_H
#define ICONTROLLER_H

#include <SFML/Window.hpp>

namespace gamefw {

class IController
{
public:
    virtual void keyPressed(sf::Event::KeyEvent& keyevent);
    virtual void keyReleased(sf::Event::KeyEvent& keyevent);
    virtual void mouseMoved(sf::Event::MouseMoveEvent event);

};

}

#endif // ICONTROLLER_H
