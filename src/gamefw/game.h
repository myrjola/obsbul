#ifndef GAME_H
#define GAME_H

#include "gamefw.h"

#include <SFML/Window.hpp>


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
    
    /**
     * @brief Adds entity to the rendering pipeline.
     *
     * @param entity ditto.
     */
    void addToRenderQueue(const Entity& entity);

    /**
     * @brief Adds pointlight to the rendering pipeline.
     *
     * @param pointlight ditto.
     * @return void
     */
    void addToPointLightQueue(const PointLight& pointlight);

	Renderer& getRenderer();

private:
	Renderer m_renderer;
    const uint m_window_middle_x;
    const uint m_window_middle_y;
    sf::ContextSettings m_main_window_context;
    sf::Window m_main_window;

    shared_ptr<GameState> m_active_gamestate;
};

}

#endif // GAME_H
