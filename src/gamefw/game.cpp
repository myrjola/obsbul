#include "../common.h"
#include "../ogl.h"

#include "game.h"
#include "renderer.h"

#include <SFML/Graphics.hpp>
#include "gamefw.h"

using namespace gamefw;

enum Windows {
    MAIN
};

Game::Game(const uint display_width, const uint display_height,
           OpenGLVersion opengl_version)
{
    if (opengl_version == OGL_3_3) {
        m_main_window_context = sf::ContextSettings(24, 8, 0, 3, 3);
        LOG(logINFO) << "Using OpenGL 3.3 Renderer.";
    } else if (opengl_version == OGL_2_1) {
        m_main_window_context = sf::ContextSettings(24, 8, 0, 2, 1);
        LOG(logINFO) << "Falling back to OpenGL 2.1.";
    } else {
        LOG(logERROR) << "No support for given OpenGL version.";
        throw OpenGLError();
    }
    int status = glewInit();
    if (GLEW_OK != status) {
        LOG(logERROR) << "Error:" << glewGetErrorString(status) << "\n";
    }

    
    m_main_window.create(sf::VideoMode(display_width, display_height,
                                       24), "Test", sf::Style::Default,
                         m_main_window_context);
    m_main_window.setActive();
    m_main_window.setKeyRepeatEnabled(false);
    m_main_window.setMouseCursorVisible(false);
    m_main_window.setVerticalSyncEnabled(true);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // Renderer must be initialized after main window because of OpenGL context dependency.
    m_renderer = shared_ptr<Renderer>(new Renderer(display_width, display_height, opengl_version));
}

Game::~Game()
{
    m_main_window.close();
}

sf::Window* Game::getMainWindow()
{
    return &m_main_window;
}

UpdateStatus Game::update()
{
    UpdateStatus status = m_active_gamestate->update();
    m_renderer->render();
    m_main_window.display();
    if (status == UPDATE_QUIT) {
        m_main_window.close();
    }
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
