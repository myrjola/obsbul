#include "../common.h"

#include "game.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <OIS/OIS.h>

using namespace gamefw;

enum Windows {
    MAIN
};

Game::Game(const uint display_width, const uint display_height)
:
m_main_window_context(24, 8, 0, 3, 3),
m_controller(NULL)
{
    m_main_window.Create(sf::VideoMode(display_width, display_height,
                                       24), "Test", sf::Style::Default,
                         m_main_window_context);
    
    sf::WindowHandle window_handle = m_main_window.GetSystemHandle();
    m_input_manager = OIS::InputManager::createInputSystem(window_handle);
    
    m_main_window.SetActive();
    m_main_window.EnableKeyRepeat(false);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

Game::~Game()
{
    m_main_window.Close();
}

sf::Window* Game::getMainWindow()
{
    return &m_main_window;
}


int Game::update()
{
    sf::Event event;
    while (m_main_window.GetEvent(event)) {
        if (event.Type == sf::Event::Closed) {
            m_main_window.Close();
            return 0;
        } else if (event.Type == sf::Event::KeyPressed) {
            switch (event.Key.Code) {
                case (sf::Key::R):
                    Locator::getShaderFactory().reloadShaders();
                    break;
                case (sf::Key::Escape):
                    m_main_window.Close();
                    return 0;
                    
                default:
                    m_controller->keyPressed(event.Key);
            }
        } else if (event.Type == sf::Event::KeyReleased) {
            m_controller->keyReleased(event.Key);
        } else if (event.Type == sf::Event::MouseMoved) {
            m_controller->mouseMoved(event.MouseMove);
        }
    }
    return 1;
}

void gamefw::Game::changeController(IController* controller)
{
    m_controller = controller;
}
