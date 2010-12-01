#ifndef GAME_H
#define GAME_H

#include <SFML/Window.hpp>
#include "igamestate.h"

namespace gamefw {

class Entity;


class Renderer;

class PointLight;

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
    UpdateStatus update();

    /**
     * @brief Changes the active GameState.
     * 
     * @param gamestate ditto.
     */
    void changeGameState(shared_ptr<IGameState> gamestate);
    
    /**
     * @brief Adds entity to the rendering pipeline.
     *
     * @param entity ditto.
     */
    void addToRenderQueue(shared_ptr<Entity> entity);

    /**
     * @brief Adds pointlight to the rendering pipeline.
     *
     * @param pointlight ditto.
     * @return void
     */
    void addToPointLightQueue(shared_ptr< PointLight > pointlight);

    shared_ptr<Renderer> getRenderer();

private:
    shared_ptr<Renderer> m_renderer;
    sf::ContextSettings m_main_window_context;
    sf::Window m_main_window;

    shared_ptr<IGameState> m_active_gamestate;
};

}

#endif // GAME_H
