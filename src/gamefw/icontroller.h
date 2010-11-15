#ifndef ICONTROLLER_H
#define ICONTROLLER_H

#include <SFML/Window.hpp>

namespace gamefw {

/**
 * @brief Interface for input processing. Used in the Game class.
 *
 * Meant to be inherited. Take a look at DefaultFirstPersonController.
 **/
class IController
{
public:
    /**
     * @brief Input processing on key press.
     *
     * @param keyevent The pressed key.
     * @return void
     **/
    virtual void keyPressed(sf::Event::KeyEvent& keyevent);
    
    /**
     * @brief Input processing on key release.
     *
     * @param keyevent The released key.
     * @return void
     **/
    virtual void keyReleased(sf::Event::KeyEvent& keyevent);

    /**
     * @brief Input processing on mouse movement.
     *
     * @param event The caught event.
     * @return void
     **/
    virtual void mouseMoved(sf::Event::MouseMoveEvent event);

};

}

#endif // ICONTROLLER_H
