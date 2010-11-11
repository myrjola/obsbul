#ifndef ICONTROLLER_H
#define ICONTROLLER_H

#include <SFML/Window.hpp>

namespace gamefw {

class IController
{
public:
    virtual void keyPressed(sf::Key::Code keycode) = 0;
    virtual void keyReleased(sf::Key::Code keycode) = 0;
    virtual void mouseMoved(sf::Event::MouseMoveEvent event) = 0;
};

}

#endif // ICONTROLLER_H
