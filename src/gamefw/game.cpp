#include "../common.h"

#include "game.h"

#include <SFML/Graphics.hpp>

using namespace gamefw;

enum Windows {
    MAIN
};

Game::Game(const uint display_width, const uint display_height)
:
m_main_window_context(24, 8, 0, 3, 3)
{
    m_main_window.Create(sf::VideoMode(display_width, display_height,
                                       24), "Test", sf::Style::Default,
                         m_main_window_context);
    m_main_window.SetActive();
    m_main_window.EnableKeyRepeat(false);
    m_main_window.ShowMouseCursor(false);
    m_main_window.UseVerticalSync(true);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // Renderer must be initialized after main window because of OpenGL context dependency.
    m_renderer = shared_ptr<Renderer>(new Renderer(display_width, display_height));
}

Game::~Game()
{
    m_main_window.Close();
}

sf::Window* Game::getMainWindow()
{
    return &m_main_window;
}

UpdateStatus Game::update()
{
    UpdateStatus status = m_active_gamestate->update();
    m_renderer->render();
    m_main_window.Display();
    return status;
}

void gamefw::Game::addToRenderQueue(shared_ptr<Entity> entity)
{
    m_renderer->addToRenderQueue(entity);
}

void gamefw::Game::addToPointLightQueue(shared_ptr<PointLight> pointlight)
{
    m_renderer->addToPointLightQueue(pointlight);
}

shared_ptr<Renderer> gamefw::Game::getRenderer()
{
    return m_renderer;
}

void gamefw::Game::changeGameState(shared_ptr< IGameState > gamestate)
{
    m_active_gamestate = gamestate;
}
