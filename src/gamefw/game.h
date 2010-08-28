#include <SFML/Window.hpp>

#include "gamefw.h"

#ifndef GAME_H
#define GAME_H

namespace gamefw {

/**
 * @brief Main game class. Provides the main window and performs input processing.
 */
class Game
{
public:
    Game();

    ~Game();

    sf::Window* getMainWindow();

    /**
     * Draws the screen and performs input processing.
     */
    void update();

private:
    sf::ContextSettings m_main_window_context;
    sf::Window m_main_window;
};

}

#endif // GAME_H
