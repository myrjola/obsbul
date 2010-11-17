#ifndef GAME_H
#define GAME_H

#include "gamefw.h"

#include <SFML/Window.hpp>

#include <OIS/OIS.h>


namespace gamefw {
    
class GameState;

/**
 * @brief Main game class. Provides the main window and performs input processing.
 */
class Game
{
public:
    Game(const uint display_width, const uint display_height);

    ~Game();

    sf::Window* getMainWindow();

    /**
     * @brief Draws the screen and performs input processing.
     */
    int update();

    /**
     * @brief Changes the active GameState.
     * 
     * @param gamestate ditto.
     */
    void changeGameState(shared_ptr<GameState> gamestate);

    void changeController(gamefw::IController* controller);

private:
    sf::ContextSettings m_main_window_context;
    sf::Window m_main_window;

    shared_ptr<GameState> m_active_gamestate;
    IController* m_controller;
    OIS::InputManager* m_input_manager;
};

}

#endif // GAME_H
